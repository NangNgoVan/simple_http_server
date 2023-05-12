--- Simple http server ---

A simple http server that support HTTP/1.1 in C++

1. Results:
    * Serve a simple website's page.
    * Handle at least 10000 concurent connections.

2. Run the project using Terminal:
```
    mkdir build
    cd build
    make
    ./main
```

    The server run on port: 8080

3. Benchmarks
- Testing environment
```
- OS: Ubuntu 22.04 installed on MacOS
- Chipset: M1 pro (4 cores in simulate )
- Ram: 4Gb
- SSD: 50Gb
```

- Using Apache benmarks:
- Simulate 10000 HTTP conections on 1s
-----------------------------
```
$ab -c10000 -t 1s localhost:8080/

Server Software:        
Server Hostname:        localhost
Server Port:            8080

Document Path:          /
Document Length:        675 bytes

Concurrency Level:      10000
Time taken for tests:   1.004 seconds
Complete requests:      34614
Failed requests:        0
Total transferred:      25622740 bytes
HTML transferred:       24189300 bytes
Requests per second:    34464.77 [#/sec] (mean)
Time per request:       290.151 [ms] (mean)
Time per request:       0.029 [ms] (mean, across all concurrent requests)
Transfer rate:          24914.33 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   48  12.6     48      93
Processing:    19   56  12.9     54     112
Waiting:        1   40  10.2     38      89
Total:         65  104  10.9    102     165

Percentage of the requests served within a certain time (ms)
  50%    102
  66%    104
  75%    107
  80%    110
  90%    116
  95%    127
  98%    137
  99%    143
 100%    165 (longest request)

---------------------------
```

* Using wrk
- Simulate 100 http connections on 10 threads in 1s
------------------
```
$ ./wrk -t100 -c100 -d1s http://127.0.0.1:8080/

Running 1s test @ http://127.0.0.1:8080/
  100 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   271.36us  381.56us  14.13ms   97.03%
    Req/Sec     1.48k     1.71k    4.33k    71.60%
  25249 requests in 1.10s, 17.22MB read
  Socket errors: connect 0, read 25249, write 0, timeout 0
Requests/sec:  22946.31
Transfer/sec:     15.65MB

```
-----------

5.References

- https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
- https://www.geeksforgeeks.org/socket-programming-cc/
- https://beej.us/guide/bgnet/html/#client-server-background
