package org.example;
import jdk.nashorn.internal.codegen.CompilerConstants;

import java.lang.Math;
import java.util.concurrent.Callable;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.Future;

public class App
{
    public static void main( String[] args ) throws Exception {
        int[] data = new int[10];
        for (int i = 0; i < 10; ++i)
            data[i] = 10 - i;

        Callable<int[]> dt = new DivideTask(data, 0, 9);
        ForkJoinPool forkJoinPool = new ForkJoinPool();

        int[] result = new int[10];
//        result = forkJoinPool.invoke(dt);
//        forkJoinPool.execute(dt);
        Future<int[]> f1 = forkJoinPool.submit(dt);

        forkJoinPool.shutdown();
        while (!forkJoinPool.isTerminated()) {}


        for (int e : f1.get()) {
            System.out.printf("%d | ", e);
        }
        System.out.println();
    }
}
