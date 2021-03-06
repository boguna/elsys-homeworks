package homework3;

public class Homework3 {
	private static void printProperty(String name) {
		String out = System.getProperty(name);
		if(out != null) System.out.println(out);
	}
	public static void main(String[] args) {
		if(args.length == 0) {
			System.err.println("No arguments provided");
			System.exit(1);
		}

		int end = args.length-1;
		if(args[end].equals("down")) {
			for(int i=end; i>=0; i--) {
				printProperty(args[i]);
			}
		} else if(args[end].equals("up")) {
			for(int i=0; i<end; i++) {
				printProperty(args[i]);
			}
		} else {
			for(int i=0; i<args.length; i++) {
				printProperty(args[i]);
			}
		}
	}
}

