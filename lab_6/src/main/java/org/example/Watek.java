package org.example;

public class Watek extends Thread{
    static int counter = 0;
    int t_id;
    int pos;
    Obraz image;

    public Watek(int pos, Obraz image) {
        this.pos = pos;
        this.image = image;
        this.t_id = counter++;
    }

    @Override
    public void run() {
        image.calculateHistogramInParallel(pos);
    }
}
