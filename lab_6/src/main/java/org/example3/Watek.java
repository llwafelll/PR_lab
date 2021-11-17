package org.example3;

public class Watek extends Thread{
    int t_id;
    int start, stop;
    Obraz image;

    public Watek(int threadId, int start, int stop, Obraz image) {
        this.image = image;
        this.t_id = threadId;
        this.start = start;
        this.stop = stop;
    }

    @Override
    public void run() {
        image.calculateHistogramParallel(t_id, start, stop);
    }
}
