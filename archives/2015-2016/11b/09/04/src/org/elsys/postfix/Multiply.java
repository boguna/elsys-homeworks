package org.elsys.postfix;

public class Multiply extends BinaryOperation {
	public Multiply() {
		super("*");
	}

	@Override
	public double calc(double v1,double v2) {
		double res = v2 * v1;
		System.out.println("R: " + res);
		if(BinaryTest.status == false){
			getContext().push(res);
		}
		return res;
	}
}
