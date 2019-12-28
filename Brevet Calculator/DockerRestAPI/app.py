import os
import flask
from flask import Flask, flash,redirect, url_for, request, render_template, make_response
from flask_restful import Resource, Api
from pymongo import MongoClient
import arrow  # Replacement for datetime, based on moment.js
import acp_times  # Brevet time calculations
import config
from bson.json_util import dumps


import logging

app = Flask(__name__)
api = Api(app)
CONFIG = config.configuration()
app.secret_key = CONFIG.SECRET_KEY

client = MongoClient("172.18.0.2", 27017)
db = client.tododb

db.tododb.remove({})    #start with an empty db at startup
control_data = {}

@app.route("/")
@app.route("/index")
def index():
    control_data.clear()
    app.logger.debug("Main page entry")
    return render_template('calc.html')

@app.route('/empty')
def empty():
    return render_template('emptytimes.html')

@app.route('/submit')
def submit():
    app.logger.debug("CLEARING DB")
    db.tododb.remove({})
    app.logger.debug("ADDING ITEMS TO DB")
    if len(control_data) == 0:
        return redirect(url_for('empty'))

    for entry in control_data.values():
        item_doc = {
            'km': entry[0],
            'open': entry[1],
            'close': entry[2]
        }
        db.tododb.insert_one(item_doc)
    _items = db.tododb.find()
    items = [item['open'] for item in _items]
    app.logger.debug(items)


    return ('', 204)

@app.route('/display')
def display():
    app.logger.debug("Redirecting to display page")
    _items = db.tododb.find()
    items = [item for item in _items]
    return render_template('todo.html', items=items)

@app.errorhandler(404)
def page_not_found(error):
    app.logger.debug("Page not found")
    flask.session['linkback'] = flask.url_for("index")
    return flask.render_template('404.html'), 404


# The three basic APIs

class listAll(Resource):
    def get(self):
        _items = db.tododb.find({},{"_id":0,"km":1,"open":1,"close":1}).sort('$natural')
        items = [item for item in _items]
        return ({"Brevets": items})

api.add_resource(listAll, '/listAll')

class listOpenOnly(Resource):
    def get(self):
        _items = db.tododb.find({},{"_id":0,"close":0}).sort('$natural')
        items = [item for item in _items]
        return ({"open_times": items})

api.add_resource(listOpenOnly, '/listOpenOnly')

class listCloseOnly(Resource):
    def get(self):
        _items = db.tododb.find({},{"_id":0,"open":0}).sort('$natural')
        items = [item for item in _items]
        return ({"close_times": items})

api.add_resource(listCloseOnly, '/listCloseOnly')

# for the format specific APIs, output_csv creates the representation for csv files. An extension of Flask RESTful
# outputs csv response, w each brevet on new line starting with brevet distance then indicated value: open, close, or both
api.representation('text/csv')
def output_csv(data,code,headers=None):
        csvstr = ""
        for brevet in data:
            l = len(brevet)
            i = 1
            for val in brevet.values():
                if i != l:
                    st = str(val) + ', '
                else:
                    st = str(val) + '<br/>'
                csvstr+=st
                i+=1
        resp = make_response(csvstr,code)
        resp.headers.extend(headers or {})
        return resp


class listAllformat(Resource):
    def get(self,format):
        k = request.args.get('top')
        if k!=None:
            _items = db.tododb.find({},{"_id":0}).sort('$natural').limit(int(k))
        else:
            _items = db.tododb.find({},{"_id":0}).sort('$natural')
        items = [item for item in _items]
        if format == "json":
            return ({"Brevets": items})
        else:
            csv = output_csv(items, 200)
            return csv

api.add_resource(listAllformat,'/listAll/<format>')

class listOpenOnlyformat(Resource):
    def get(self,format):
        k = request.args.get('top')
        if k!=None:
            _items = db.tododb.find({},{"_id":0,"close":0}).sort('$natural').limit(int(k))
        else:
            _items = db.tododb.find({},{"_id":0,"close":0}).sort('$natural')
        items = [item for item in _items]
        if format == "json":
            return ({"Open Times": items})
        else:
            csv = output_csv(items, 201)
            return csv

api.add_resource(listOpenOnlyformat,'/listOpenOnly/<format>')

class listCloseOnlyformat(Resource):
    def get(self,format):
        k = request.args.get('top')
        if k!=None:
            _items = db.tododb.find({},{"_id":0,"open":0}).sort('$natural').limit(int(k))
        else:
            _items = db.tododb.find({},{"_id":0,"open":0}).sort('$natural')
        items = [item for item in _items]
        if format == "json":
            return ({"Close Times": items})
        elif format == "csv":
            csv = output_csv(items, 201)
            return csv

api.add_resource(listCloseOnlyformat,'/listCloseOnly/<format>')

###############
#
# AJAX request handlers
#   These return JSON, rather than rendering pages.
#
###############
@app.route("/_calc_times")
def _calc_times():
    """
    Calculates open/close times from miles, using rules
    described at https://rusa.org/octime_alg.html.
    Expects one URL-encoded argument, the number of miles.
    """
    app.logger.debug("Got a JSON request")
    km = request.args.get('km', 999, type=float)
    app.logger.debug("km={}".format(km))
    app.logger.debug("request.args: {}".format(request.args))

    bd = request.args.get('bd', type=int)
    st = request.args.get('st', type=str)
    sd = request.args.get('sd',type=str)
    id = request.args.get('id',type=int)

    app.logger.debug("Evaluating start and end time for index: " + str(id))
    accpt_dist = True
    if (km > bd*1.20):
        open_time = 0
        close_time = 0
        accpt_dist = False
    else:
        # convert st from string to arrow
        time = sd + " " + st
        start_time = arrow.get(time,'YYYY-MM-DD HH:mm')

        open_time = acp_times.open_time(km, bd, start_time.isoformat())
        close_time = acp_times.close_time(km, bd, start_time.isoformat())

        # update the dictionary with km,open_time,close_time for that specific row.
        # goes by index so revised rows are not entered more than once, only current value is kept
        s_time = arrow.get(open_time).format('YYYY-MM-DD HH:mm')
        e_time = arrow.get(close_time).format('YYYY-MM-DD HH:mm')
        control_data[id]=[km,s_time,e_time]

    result = {"open": open_time, "close": close_time, "acceptable": accpt_dist}

    return flask.jsonify(result=result)

#############

app.debug = CONFIG.DEBUG
if app.debug:
    app.logger.setLevel(logging.DEBUG)

if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
