package org.example;

import java.util.concurrent.Callable;
import java.util.concurrent.RecursiveTask;

class DivideTask extends RecursiveTask<int[]> implements Callable<int[]> {
 
  int[] arrayToDivide;
  private int index_start;
  private int index_stop;
//  int[] scal_tab; // This should be here?

  public DivideTask(int[] arrayToDivide, int index_start, int index_stop) {
    this.arrayToDivide = arrayToDivide;
    this.index_start = index_start;
    this.index_stop = index_stop;
  }
 
  protected int[] compute() {

      if ((index_stop - index_start) <= 1)
        return arrayToDivide;

      int index_mid = (index_start + index_stop) / 2;
 
      DivideTask task1 = new DivideTask(arrayToDivide, index_start, index_mid - 1);
      DivideTask task2 = new DivideTask(arrayToDivide, index_mid, index_stop);

      // .......
      task1.fork();
      task2.fork();

      //Wait for results from both tasks
      int[] tab1 = task1.join();
      int[] tab2 = task2.join();

      int[] scal_tab = new int[tab1.length + tab2.length];
      scal_tab(tab1, tab2, scal_tab);
      return scal_tab;

  }

  private void scal_tab(
          int[] tab1,
          int[] tab2,
          int[] scal_tab) {
     
    int i = 0, j = 0, k = 0;
     
    while ((i < tab1.length) && (j < tab2.length)) {
     
      if (tab1[i] < tab2[j]) {
        scal_tab[k] = tab1[i++];
      } else {
        scal_tab[k] = tab2[j++];
      }
       
      k++;
    }
     
    if (i == tab1.length) {
       
      for (int a = j; a < tab2.length; a++) {
        scal_tab[k++] = tab2[a];
      }
       
    } else {
       
      for (int a = i; a < tab1.length; a++) {
        scal_tab[k++] = tab1[a];
      }
       
    }
  }

    @Override
    public int[] call() throws Exception {
        return compute();
    }
}
