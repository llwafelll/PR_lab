package org.example;
import java.lang.Math;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args ) throws Exception {
        double a = 0, b = Math.PI, dx = .001;
        Calka_callable i1 = new Calka_callable(a, b, dx);

        System.out.printf("\nIntegral[%.2f, %.2f] sin(x) dx = %.15f\n", a, b, i1.call());
    }
}
