"""
Open and close time calculations
for ACP-sanctioned brevets
following rules described at https://rusa.org/octime_alg.html
and https://rusa.org/pages/rulesForRiders
"""
import arrow

#  Note for CIS 322 Fall 2016:
#  You MUST provide the following two functions
#  with these signatures, so that I can write
#  automated tests for grading.  You must keep
#  these signatures even if you don't use all the
#  same arguments.  Arguments are explained in the
#  javadoc comments.
#

BREVET_TIMES = [(0,200,15,34),(200,400,15,32),(400,600,15,30),(600,1000,11.428,28)]
END_TIMES = [(200.0,13,30),(300.0,20,0),(400.0,27,0),(600.0,40,0),(1000.0,75,0)]
def open_time(control_dist_km, brevet_dist_km, brevet_start_time):
    """
    Args:
       control_dist_km:  number, the control distance in kilometers
       brevet_dist_km: number, the nominal distance of the brevet
           in kilometers, which must be one of 200, 300, 400, 600,
           or 1000 (the only official ACP brevet distances)
       brevet_start_time:  An ISO 8601 format date-time string indicating
           the official start time of the brevet
    Returns:
       An ISO 8601 format date string indicating the control open time.
       This will be in the same time zone as the brevet start time.
    """

    hrs = 0
    mins = 0
    if control_dist_km > brevet_dist_km:
        control_dist_km = brevet_dist_km
    for control_time in BREVET_TIMES:
        l1,l2,min,max = control_time
        if (control_dist_km == 0.0):
            continue
        elif control_dist_km >= (l2-l1):
            whole = (l2-l1)/max
            h = (l2-l1)//max
            m = round((whole - h)*60)
            control_dist_km = control_dist_km - (l2-l1)
        elif control_dist_km < (l2-l1):
            whole = control_dist_km/max
            h = control_dist_km//max
            m = round((whole-h)*60)
            control_dist_km = 0.0

        hrs = int(hrs + h)
        mins = mins + m
        while (mins > 60):
            hrs+=1
            mins = mins%60

    start_time = arrow.get(brevet_start_time, 'YYYY-MM-DDTHH:mm')
    start_time = start_time.shift(hours=+hrs)
    start_time = start_time.shift(minutes=+mins)

    print("START TIME: " + str(start_time.time()))

    return start_time.isoformat()


def close_time(control_dist_km, brevet_dist_km, brevet_start_time):
    """
    Args:
       control_dist_km:  number, the control distance in kilometers
          brevet_dist_km: number, the nominal distance of the brevet
          in kilometers, which must be one of 200, 300, 400, 600, or 1000
          (the only official ACP brevet distances)
       brevet_start_time:  An ISO 8601 format date-time string indicating
           the official start time of the brevet
    Returns:
       An ISO 8601 format date string indicating the control close time.
       This will be in the same time zone as the brevet start time.
    """
    hrs = 0
    mins = 0

    if control_dist_km == 0.0:
        hrs += 1
    elif control_dist_km >= brevet_dist_km:
        for end_time in END_TIMES:
            dist,hours,minutes = end_time
            if (brevet_dist_km == dist):
                hrs = hours
                mins = minutes
    elif (control_dist_km > 600):
        hrs += 40
        control_dist_km -=600

        whole = control_dist_km/11.428
        h = control_dist_km//11.428
        mins += round((whole-h)*60)
        hrs+=int(h)
        control_dist_km = 0.0

    else:
        whole = control_dist_km/15.0
        h = control_dist_km//15.0
        mins += round((whole-h)*60)
        hrs+=int(h)

    while (mins > 60):
        hrs+=1
        mins = mins%60

    end_time = arrow.get(brevet_start_time, 'YYYY-MM-DDTHH:mm')
    end_time = end_time.shift(hours=+hrs)
    end_time = end_time.shift(minutes=+mins)
    print("END TIME: " + str(end_time.time()))

    return end_time.isoformat()
