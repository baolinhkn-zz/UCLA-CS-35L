NAME: Baolinh Nguyen
UID: 104 732 121
TA: Farnoosh Javadi
CS35L Lab 6
FILE: readme.txt

As I was implementing a multithreaded version of SRT, I ran into several
issues. For one, I was had some issues translating the function parameters of
pthread_create and pthread_join. Many of those issues were resolved as I
referenced the TA slides that my TA gave me. 

As I was creating the function to parallelize, I saw that there were some
variables I used both outside and inside the function. It took me some time to
realize that those variables would need to be global variables. Moreover, I knew
that I had to somehow keep the output of the color calculations. I did this by
creating a 3D array. However, I ran into some issues as I was trying to convert
all instances of the formerly 1D array into a 3D array for instance:
main.c:238:95: warning: pointer/integer type mismatch in conditional expression
[enabled by default]
                 scaled_color[px][py][i] = max( min(scaled_color[i], 255), 0);
There would be times I would forget to change all instances of the scaled_color
array but those were quick fixes.

I ran into issues with segmentation faults at times as well when I was creating
the threads.
   for (i = 0; i < nthreads; i++)
      {
        ret = pthread_create(&threads[i], NULL, createImage, &threadIDs[i]);
        if (ret != 0)
          {
            printf("Error creating thread. Error number %d\n", ret);
            exit(1);
          }
      }
This was formerly:
  for (i = 0; i < nthreads; i++)
      {
        ret = pthread_create(&threads[i], NULL, createImage, (void*) i);
        if (ret != 0)
          {
            printf("Error creating thread. Error number %d\n", ret);
            exit(1);
          }
      }
This would give me a segmentation fault. This was because I had initially tried
to pass in the value of i itself and as I dereference it in my createImage
function, I would get a segmentation fault. Instead, I used a pointer with a
value of i by creating an array of threadIDs to use.

Otherwise, the only other issues I had were simple mistakes; for instance,
forgetting to add a return NULL; statement to the end of my createImage()
function.

If I look at the times of all the thread combinations:
time ./srt 1-test.ppm >1-test.ppm.tmp
real 0m46.253s
user 0m46.245s
sys  0m0.002s

time ./srt 2-test.ppm >2-test.ppm.tmp
real 0m22.262s
user 0m44.308s
sys  0m0.002s

time ./srt 4-test.ppm >4-test.ppm.tmp
real 0m11.313s
user 0m44.747s
sys  0m0.004s

time ./srt 8-test.ppm >8-test.ppm.tmp
real 0m7.164s
user 0m46.242s
sys  0m0.003s

It is clear that the benefits of threads shows in the real time comparisons. For
instance, if we look at the real time of a single thread (46.253) and the real
time of a fucntion threaded with 8 threads (7.164), the result is nearly 6 times
as fast. 
