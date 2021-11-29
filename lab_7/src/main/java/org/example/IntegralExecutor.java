package org.example;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.lang.Math;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

import static java.lang.System.currentTimeMillis;

public class IntegralExecutor
{
    private static final int NTHREADS = 8;

    public static void main( String[] args ) throws Exception {

        // Initialization of the integral interval and dx (height)
        double a = 0, b = Math.PI;
//        double dx = 1e-3;
//        double dx = 1e-6;
        double dx = 1e-8;

        // Create executor
        ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);

        // Declare ArrayList containing futures (calculated integral value of interval it received)
        List<Future<Double>> integralFutures = new ArrayList<Future<Double>>();

        // Create sub-intervals and submit them to threads in the threadPool
//        final int tasks = 10;
//        final int tasks = 50;
        final int tasks = 100;
        double delta = (b - a) / tasks;
        double partA, partB;

        // TIME START
        long t_par;
        t_par = System.currentTimeMillis();

        for (int i = 0; i < tasks; ++i) {
            partA = i * delta;
            partB = (i + 1) * delta;

            Callable<Double> partIntegral = new Calka_callable(partA, partB, dx);
            Future<Double> partIntegralFuture = executor.submit(partIntegral);

            integralFutures.add(partIntegralFuture);
        }

        // This will make the executor accept no new threads
        // and finish all existing threads in the queue
        executor.shutdown();

        // Wait until all threads finish
        while (!executor.isTerminated()) {}
//        System.out.println("\t>>>Finished all threads<<<\n");

        // Sum up all resluts from partial integrals
        double integralParallel = 0; // contains sum of all partial integrals
        for (Future<Double> partResult : integralFutures) {
            try {
                integralParallel += partResult.get();
            }  catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }

        // TIME STOP
        t_par = currentTimeMillis() - t_par;


        // Calculate integral sequentially
        double integralSequential = 0;

        // TIME START
        long t_seq;
        t_seq = currentTimeMillis();

        Calka_callable i1 = new Calka_callable(a, b, dx);
        integralSequential = i1.call();

        // TIME STOP
        t_seq = System.currentTimeMillis() - t_seq;

        // Print integrals results
        System.out.printf("\nIntegral[%.2f, %.2f] sin(x) dx = %.15f\n", a, b, integralParallel);
        System.out.printf("\nIntegral[%.2f, %.2f] sin(x) dx = %.15f\n", a, b, integralSequential);

        System.out.printf("\nDelta = %.20f\n", Math.abs(integralSequential - integralParallel));
        System.out.printf("time par: %dms\n", t_par);
        System.out.printf("time seq: %dms\n", t_seq);

        String stringLog = String.format("%d; %d; %.8f; %d; %d\n", NTHREADS, tasks, dx, t_seq, t_par);
        System.out.print(stringLog);

        BufferedWriter writer = new BufferedWriter(new FileWriter("logs.txt", true));
        writer.append(stringLog);
        writer.close();
    }
}
