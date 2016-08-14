#Web Crawler

To use the web fetchers, run:
`./file_tester pagea`

To run the file fetchers, run:
`./web_tester index.txt`

1. We created two groups of threads, one for downloaders and one for parsers. The downloaders will get a link from the link queue, fetch the page, and put it into page queue accordingly. The parsers will parse the page from the page queue, find the link in the page, and put the links into the link queue accordingly. 

2. The link queue is fixed-sized, where we created an array to store the links, two indices to indicate the place for incoming links and output links. Also, we use numfull to indicate the current size of the queue. We use two conditional variables and a mutex to make it thread-safe, and check if the queue is empty/full before add/poll operation.

3. The page queue is unbounded, where we created a linkedlist to store the pages, two pointers to indicate the place for incoming pages and output pages. Here we also use two conditional variables and a mutex to make it thread-safe, and check if the queue is empty before polling (the queue will never be full). 

4. We use a hashset to check if the page has already been visited. If a page has not been visited, parse the page and add the current page to the hashset. If it has already been visited, skip the parsing procedure. The reference of our hash function is https://en.wikipedia.org/wiki/Fletcher%27s_checksum  (the fletcher32 one).

5. Whenever a link is found, use the `edge()` function to let the calling function know the link.

6. We created another thread as a terminator, which keeps check if there is any item in the queue or being parsed/fetched. When there is no item in the whole program, the terminator will stop all the downloaders/parsers (all of them should be asleep), and then finish the main thread.
