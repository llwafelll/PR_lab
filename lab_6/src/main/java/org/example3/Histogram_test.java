package org.example3;

import java.util.Scanner;


public class Histogram_test {
    
    public static void main(String[] args) {

		// Get data from user
		Scanner scanner = new Scanner(System.in);
		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();

		System.out.println("Set number of threads:");
		int N = scanner.nextInt();

		Obraz obraz_1 = new Obraz(n, m);

		// Run sequentially
		obraz_1.calculate_histogram();

		// Run in parallel (scenario 2)
		final int elementPerThread = (int)(94 / N);
		Watek[] thrs = new Watek[N];

		int a, b, rest;
		for (int i = 0; i < N; ++i) {
			a = i * elementPerThread;
			b = (i + 1) * elementPerThread;

			rest = 94 - (elementPerThread * N);
			if (i == N - 1)
				b += rest;

			(thrs[i] = new Watek(i, a, b, obraz_1)).start();
		}

		for (int i = 0; i < N; ++i) {
			try {
				thrs[i].join();
			} catch (InterruptedException e) {
				System.out.printf("Exception caught: %s\n", e);
			}
		}

		// Print the results
		System.out.println("Histogram generated sequentially:");
		obraz_1.print_histogram();
		System.out.println("Histogram generated parallel (scenario 2):");
		obraz_1.printHistParallel();
		obraz_1.graph(N);
		obraz_1.graphOld();

		// Test integrity
		System.out.println("Histogram comparison result:");
		System.out.println(obraz_1.compareHist());
	}
}

