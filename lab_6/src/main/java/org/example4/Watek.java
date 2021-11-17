package org.example4;

public class Watek extends Thread{
    int t_id;
    int start, stop;
    Obraz image;

    public Watek(int start, int stop, int threadId, Obraz image) {
        this.image = image;
        this.t_id = threadId;
        this.start = start;
        this.stop = stop;
    }

    @Override
    public void run() {
        image.calculateForRow(start, stop, t_id);
    }
}
