# Quacker Server

A simple publisher/subscriber server that can handle the work of many publishers and many subscribers running at the same time. Entries were published to circular ring buffers for each topic. Like the photo sharing websites of today, the Quacker server is meant to display the content pushed by publishers that is then accessed by subscribers. To free up space in the topic buffers, a cleaning thread accessed each queue and cleared out old content so more content could be pushed to the queue. Thread synchronization was a big task in this project, because with multiple threads wanting access to the same resource we must avoid race conditions.

## Function

Reads configuration file which contains the commands to spawn new publisher and subscriber threads from a pool of proxy threads. It assigns work to each thread based on each threads individual configuration file. The work can include adding a new photo to a topic queue or "reading" a new photo from a queue. A cleaner thread clears up space in the topic queues intermittently so new "posts" can be added to the topics if they are full. 

As each subscriber thread consumes from the topic queues, a new HTML page is made per subscriber to display the posts that they have consumed. These pages will appear during run time. 

## Usage

To utilize this server:

Clone this directory to your local machine
"make"
"./server commands.txt"

To view consumer webpages, click the corresponding HTML pages inside the directory. 

## Author

Claire Kolln
email: ckolln@cs.uoregon.edu

## For CIS 415: Operating Systems
