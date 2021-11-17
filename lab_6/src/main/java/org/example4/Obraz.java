package org.example4;

import java.util.Random;


class Obraz {
    
    private int size_n;
    private int size_m;
	private int Nthreads;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;
	private int[][] histParallel;
	private int[] rowToThread;


	public Obraz(int n, int m, int N) {
	
		this.size_n = n;
		this.size_m = m;
		this.Nthreads = N;
		tab = new char[n][m];
		tab_symb = new char[94];
		rowToThread = new int[n];

		final Random random = new Random();

		// for general case where symbols could be not just integers
		for(int k=0;k<94;k++) {
			tab_symb[k] = (char)(k+33); // substitute symbols
	}

	for(int i=0;i<n;i++) {
	    for(int j=0;j<m;j++) {	
		tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-127
		//tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
		System.out.print(tab[i][j]+" ");
	    }
	    System.out.print("\n");
	}
	System.out.print("\n\n"); 
	
	histogram = new int[94];
	histParallel = new int[Nthreads][94];
   	clear_histogram();
	clearHistogramParallel();
    }
    
    public void clear_histogram(){
		for(int i=0;i<94;i++) histogram[i]=0;
    }

	public void clearHistogramParallel(){
		for (int i = 0; i < 94; ++i)
			for (int j = 0; j < Nthreads; ++j)
				histParallel[j][i] = 0;
	}

	public void calculate_histogram(){
		for(int i=0;i<size_n;i++) {
			for(int j=0;j<size_m;j++) {

			// optymalna wersja obliczania histogramu, wykorzystująca fakt, że symbole w tablicy
			// można przekształcić w indeksy tablicy histogramu
			// histogram[(int)tab[i][j]-33]++;

			// wersja bardziej ogólna dla tablicy symboli nie utożsamianych z indeksami
			// tylko dla tej wersji sensowne jest zrównoleglenie w dziedzinie zbioru znaków ASCII
			for(int k=0;k<94;k++) {
				if(tab[i][j] == tab_symb[k]) histogram[k]++;
				//if(tab[i][j] == (char)(k+33)) histogram[k]++;
			}

			}
		}
    }

	public void calculateForRow(int start, int stop, int threadId) {
		System.out.printf("Running calculateForRow for start=%d and stop=%d\n", start, stop);
		for (int i = start; i < stop; ++i)
			for (int j = 0; j < size_m; ++j)
				for (int k = 0; k < 94; ++k)
					if (tab[i][j] == tab_symb[k]) {
//						histParallel[(int)(tab[i][j] - 33)]++;
						histParallel[threadId][(int)(tab[i][j] - 33)]++;
						rowToThread[i] = threadId;
					}

//		for (int i = 0; i < size_n; ++i)
//			for (int j = 0; j < size_m; ++j)
//				for (int k = start; k < stop; ++k)
//					if (tab[i][j] == tab_symb[k]) {
//						histParallel[k]++;
//						charToThreadId[k] = threadId;
//					}

//				if (tab[i][j] == (char)(pos + 33)) {
//					histParallel[pos]++;
//					thread_ids[pos] = pos;
//				}
	}

    public void print_histogram(){
		for(int i=0;i<94;i++) {
			if (histogram[i] != 0)
				System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}
    }

	public void printHistParallel() {
		int printed = 0;
		int sum;

		for(int i=0;i<94;i++) {
			sum = 0;
			for (int j = 0; j < Nthreads; ++j) {
				sum += histParallel[j][i];
			}

			if (sum != 0) {
				System.out.print(tab_symb[i] + " " + sum + "  |  ");
				if (++printed % 5 == 0) System.out.println();
			}
		}

		System.out.println();
	}

	// TODO: do that as well
//	public void graphOld() {
//		StringBuilder str = new StringBuilder("");
//		StringBuilder str2 = new StringBuilder("");
//		for (int i = 0; i < 94; ++i) {
//			if (histParallel[i] != 0) {
//				str = new StringBuilder("");
//				str2 = new StringBuilder("");
//
//				for (int j = 0; j < histParallel[i]; ++j) {
//					str.append("=");
//				}
//
//				for (int j = 0; j < rowToThread[i]; ++j) {
//					str2.append(histParallel[j]);
//				}
//
//				System.out.printf("Watek %3d: %10s :[%-40s]\n", rowToThread[i], tab_symb[i], str.toString());
//			}
//		}
//	}

	public void graph() {
		/* Print thread number and characters that were associated with this thread (in order to count)
		 * show now many chars in total was encountered using = signs
		 * Output example:
		 * Thread no. 000. Assigned chars: $(*-8@ELN
		 * [==========                                                                      ]
		 * */
		StringBuilder str = new StringBuilder("");
		StringBuilder str2 = new StringBuilder("");

		for (int j = 0; j < Nthreads; ++j) {
			str = new StringBuilder("");
			str2 = new StringBuilder("");

			for (int i = 0; i < 94; ++i) {
				if (histParallel[j][i] != 0) {
					str2.append(tab_symb[i]);
					for (int k = 0; k < histParallel[j][i]; ++k) {
						str.append("=");
					}
				}
			}
			System.out.printf("Thread no. %03d. Assigned chars: %-40s\n[%-80s]\n\n",
					j, str2.toString(), str.toString());
		}
	}

	public boolean compareHist() {
		int sum;

		for (int i = 0; i < 94; ++i) {
			sum = 0;

			for (int j = 0; j < Nthreads; ++j)
				sum += histParallel[j][i];

			if (histogram[i] != sum) return false;
		}

		return true;
	}

}
