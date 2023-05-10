--- Simple http server ---

A simple http server that support HTTP/1.1 in C++

1. Current features:
    * Serve a simple website's page.
    * Handle at least 10000 concurent connections.
    * Serve 100000 requests/second

2. Run the project using Terminal:
```
    mkdir build
    cd build
    make
    ./main
```

    The server run on port: 8080

3. Test using Apache benmarks:
- Test 1: Testing with 10 concurent connections
-----------------------------
```
nangngo@Nang-Ngos-MBP ~ % ab -c 10 -n 10 127.0.0.1:8080/     
This is ApacheBench, Version 2.3 <$Revision: 1901567 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient).....done


Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8080

Document Path:          /
Document Length:        675 bytes

Concurrency Level:      10
Time taken for tests:   0.008 seconds
Complete requests:      10
Failed requests:        0
Total transferred:      7150 bytes
HTML transferred:       6750 bytes
Requests per second:    1226.99 [#/sec] (mean)
Time per request:       8.150 [ms] (mean)
Time per request:       0.815 [ms] (mean, across all concurrent requests)
Transfer rate:          856.74 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       1
Processing:     1    4   1.3      4       6
Waiting:        1    4   1.4      4       6
Total:          2    4   1.4      5       6

Percentage of the requests served within a certain time (ms)
  50%      5
  66%      5
  75%      5
  80%      6
  90%      6
  95%      6
  98%      6
  99%      6
 100%      6 (longest request)
 ```

---------------------------

- Test 2: 


