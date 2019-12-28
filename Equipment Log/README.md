# Equipment Log

This is an equipment log created to address a need in UO housing to be able to check equipment in and out efficiently to UO Housing Residents. 
Includes a Qt GUI, SQLite Database, and C++.

# The database: 2 tables

- the complete list of equipment (~300 items): name, version, type, is_out (0 or 1), loan time
- the currently checked out items: user name, id number, time it was checked out, equipment name, equipment version, equipment type
 

# Usage

User can select 3 options from the main screen

- Load/Reload: loads or reloads the table of items that are currently checked out and the list of items that are late

- Checkin: loads checkin.ui and allows the user to select items that are currently out through a series of comboboxes, prints message based on the attempt's success

- CheckOut: loads checkout.ui and allows the user to input the resident's name and ID number, and select the item from a series of comboboxes. Will only allow successful checkout if the input values are valid (9 digit ID num, non-empty name inputs, valid equipment version) 

# Roadmap

Future goals with this project include adding the capability to add new equipment items to the database, or to delete them if they get lost/never returned

# Project Status

Development has stopped as of 16 March 2019

# Author

Claire Kolln
ckolln@cs.uoregon.edu


