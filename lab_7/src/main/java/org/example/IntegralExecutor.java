package org.example;
import java.lang.Math;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

/**
 * Hello world!
 *
 */
public class IntegralExecutor
{
    private static final int NTHREADS = 10;

    public static void main( String[] args ) throws Exception {

        // Initialization of the integral interval and dx (height)
        double a = 0, b = Math.PI, dx = .001;

        // Create executor
        ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);

        // Declare ArrayList containing futures (calculated integral value of interval it received)
        List<Future<Double>> integralFutures = new ArrayList<Future<Double>>();


        double delta = (b - a) / NTHREADS;

        // Create sub-intervals and submit them to threads in the threadPool
        double partA, partB;
        for (int i = 0; i < NTHREADS; ++i) {
            partA = i * delta;
            partB = (i + 1) * delta;

            Callable<Double> partIntegral = new Calka_callable(partA, partB, dx);
            Future<Double> partIntegralFuture = executor.submit(partIntegral);

            integralFutures.add(partIntegralFuture);
        }

        double integralParallel = 0;
        for (Future<Double> partResult : integralFutures) {
            try {
                integralParallel += partResult.get();
            }  catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }

        // This will make the executor accept no new threads
        // and finish all existing threads in the queue
        executor.shutdown();

        // Wait until all threads finish
        while (!executor.isTerminated()) {}

        System.out.println("Finished all threads");
        System.out.printf("\nIntegral[%.2f, %.2f] sin(x) dx = %.15f\n", a, b, integralParallel);


        double integralSequential = 0;
        Calka_callable i1 = new Calka_callable(a, b, dx);
        System.out.printf("\nIntegral[%.2f, %.2f] sin(x) dx = %.15f\n", a, b, (integralSequential = i1.call()));

        System.out.printf("\nDelta = %.20f\n", Math.abs(integralSequential - integralParallel));
    }
}
