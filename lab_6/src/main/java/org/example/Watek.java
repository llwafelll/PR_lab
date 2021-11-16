package org.example;

public class Watek extends Thread{
    int pos;
    Obraz image;

    public Watek(int pos, Obraz image) {
        this.pos = pos;
        this.image = image;
    }

    @Override
    public void run() {
        image.calculateHistogramForGivenChar(pos);
    }
}
