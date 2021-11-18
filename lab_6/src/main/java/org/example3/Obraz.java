package org.example3;

import java.util.Random;


class Obraz {
    
    private int size_n;
    private int size_m;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;
	private int[] histParallel;
	private int[] charToThreadId;


	public Obraz(int n, int m) {
	
		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[94];
		charToThreadId = new int[94];

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
	histParallel = new int[94];
   	clear_histogram();
	clearHistogramParallel();
    }
    
    public void clear_histogram(){
		for(int i=0;i<94;i++) histogram[i]=0;
    }

	public void clearHistogramParallel(){
		for(int i=0;i<94;i++) histParallel[i]=0;
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

	public void calculateHistogramParallel(int threadId, int start, int stop) {
		for (int i = 0; i < size_n; ++i)
			for (int j = 0; j < size_m; ++j)
				for (int k = start; k < stop; ++k)
					if (tab[i][j] == tab_symb[k]) {
						histParallel[k]++;
						charToThreadId[k] = threadId;
					}
	}

	public void print_histogram(){
		int printed = 0;
		for(int i=0;i<94;i++) {
			if (histogram[i] != 0) {
				System.out.print(tab_symb[i] + " " + histogram[i] + "  |  ");
				if (++printed % 5 == 0) System.out.println();
			}
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}
		System.out.println();
	}

	public void printHistParallel() {
		final int rows = 5;
		int printed = 0;

		for(int i=0;i<94;i++) {
			if (histParallel[i] != 0) {
				System.out.print(tab_symb[i] + " " + histParallel[i] + "  |  "); ++printed;
				if (printed % 5 == 0) System.out.println();
			}
		}

		System.out.println();
	}

	public void graphOld() {
		/* Print thread number and characters that were associated with this thread (in order to count)
		 * show now many chars in total was encountered using = signs
		 * Output example:
		 * Thread no. 000. Assigned chars: $(*-8@ELN
		 * [==========                                                                      ]
		 * */
		StringBuilder str = new StringBuilder("");
		StringBuilder str2 = new StringBuilder("");
		for (int i = 0; i < 94; ++i) {
			if (histParallel[i] != 0) {
				str = new StringBuilder("");
				str2 = new StringBuilder("");

				for (int j = 0; j < histParallel[i]; ++j) {
					str.append("=");
				}

				for (int j = 0; j < charToThreadId[i]; ++j) {
					str2.append(histParallel[j]);
				}

				System.out.printf("Watek %3d: %10s :[%-40s]\n",
						charToThreadId[i], tab_symb[i], str.toString());
			}
		}
	}

	public void graph(int N_threads) {
		/* Print each char separately and provide information about the thread by which one has been encountered
		* show how many times the char was encountered using = sign
		* Output example:
		* Watek   0:          $ :[=                                       ]
		* Watek   0:          ( :[=                                       ]
		* */
		StringBuilder str = new StringBuilder("");
		StringBuilder str2 = new StringBuilder("");

		for (int i = 0; i < N_threads; ++i) {
			str = new StringBuilder("");
			str2 = new StringBuilder("");

			for (int j = 0; j < 94; ++j) {
				if (histParallel[j] != 0) {
					if (i == charToThreadId[j]) {
						for (int k = 0; k < histParallel[j]; ++k)
							str.append("=");
						str2.append(tab_symb[j]);
					}
				}
			}
			System.out.printf("Thread no. %03d. Assigned chars: %-40s\n[%-80s]\n\n",
							  i, str2.toString(), str.toString());
		}
	}

	public boolean compareHist() {
		for (int i = 0; i < 94; ++i)
			if (histogram[i] != histParallel[i]) return false;

		return true;
	}

}
