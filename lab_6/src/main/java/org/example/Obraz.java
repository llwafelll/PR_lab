package org.example;

import  java.util.Random;


class Obraz {
    
    private int size_n;
    private int size_m;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;
	private int[] histParallel;
	private int[] thread_ids;


	public Obraz(int n, int m) {
	
		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[94];
		thread_ids = new int[94];

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
		for(int i = 0; i < size_n; i++)
			for(int j = 0; j < size_m; j++)
				for(int k = 0; k < 94; k++)
					if(tab[i][j] == tab_symb[k]) histogram[k]++;
    }

	public void calculateHistogramForGivenChar(int pos) {
		for(int i=0;i<size_n;i++) {
			for (int j = 0; j < size_m; j++) {
				if (tab[i][j] == (char)(pos + 33)) {
					histParallel[pos]++;
					thread_ids[pos] = pos;
				}
			}
		}
	}

    public void print_histogram(){
		for(int i=0;i<94;i++) {
			if (histogram[i] != 0)
				System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}
    }

	public void printHistParallel() {
		for(int i=0;i<94;i++) {
			if (histParallel[i] != 0)
				System.out.print(tab_symb[i] + " " + histParallel[i] + "\n");
		}
	}

	public void graph() {
		StringBuilder str = new StringBuilder("");
		for (int i = 0; i < 94; ++i) {
			if (histParallel[i] != 0) {
				str = new StringBuilder("");
				for (int j = 0; j < histParallel[i]; ++j)
					str.append("=");

				System.out.printf("Watek %3d: [%-40s]\n", i, str.toString());
			}
		}
	}

	public boolean compareHist() {
		for (int i = 0; i < 94; ++i)
			if (histogram[i] != histParallel[i]) return false;

		return true;
	}

}
