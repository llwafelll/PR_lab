package org.example;

import java.util.Scanner;


public class Histogram_test {
    
    public static void main(String[] args) {

		// Get data
		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);

		// Run sequentially
		obraz_1.calculate_histogram();

		// Run parallel (scenario 1)
		final int N = 94;
		Watek[] thrs = new Watek[N];

		for (int i = 0; i < N; ++i)
			(thrs[i] = new Watek(i, obraz_1)).start();

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
		System.out.println("Histogram generated parallel (scenario 1):");
		obraz_1.printHistParallel();
		obraz_1.graph();

		// Test correctness
		System.out.println("\nHistogram comparison result:");
		System.out.println(obraz_1.compareHist());
    }

}

