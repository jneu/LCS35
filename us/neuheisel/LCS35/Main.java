package us.neuheisel.LCS35;

import java.math.BigInteger;
import java.util.Date;

public class Main {

	public static final BigInteger TWO = new BigInteger("2");
	public static final BigInteger NEGATIVE_ONE = new BigInteger("-1");

	public static BigInteger bruteForce(long t, final BigInteger n) {
		BigInteger w = TWO;

		System.out.println("brute force: " + new Date());

		while (t > 0) {
			w = w.multiply(w).mod(n);
			t--;
		}

		System.out.println("brute force: " + new Date());

		return w;
	}

	public static BigInteger montgomery(long t, final BigInteger n) {
		final int k = n.bitLength();
		final BigInteger r = TWO.pow(k);
		final BigInteger r_mask = r.subtract(BigInteger.ONE);
		final BigInteger n_prime = n.modInverse(r).multiply(NEGATIVE_ONE).mod(r);

		BigInteger w = TWO.multiply(r).mod(n);

		System.out.println("montgomery: " + new Date());

		while (t > 0) {
			w = w.multiply(w);
			w = w.and(r_mask).multiply(n_prime).and(r_mask).multiply(n).add(w).shiftRight(k);
			BigInteger w_smaller = w.subtract(n);
			if (w_smaller.signum() >= 0) {
				w = w_smaller;
			}

			t--;
		}

		w = w.and(r_mask).multiply(n_prime).and(r_mask).multiply(n).add(w).shiftRight(k);
		BigInteger w_smaller = w.subtract(n);
		if (w_smaller.signum() >= 0) {
			w = w_smaller;
		}

		System.out.println("montgomery: " + new Date());

		return w;
	}

	public static void main(String args[]) {
		//final BigInteger t = 10;
		//final BigInteger n = new BigInteger("253");
		//final BigInteger z = new BigInteger("13", 16);

		final long t = 1000000;
		//final long t = 79685186856218;

		final BigInteger n = new BigInteger(
			"631446608307288889379935712613129233236329881833084137558899" +
			"077270195712892488554730844605575320651361834662884894808866" +
			"350036848039658817136198766052189726781016228055747539383830" +
			"826175971321892666861177695452639157012069093997368008972127" +
			"446466642331918780683055206795125307008202024124623398241073" +
			"775370512734449416950118097524189066796385875485631980550727" +
			"370990439711973361466670154390536015254337398252457931357531" +
			"765364633198906465140213398526580034199190398219284471021246" +
			"488745938885358207031808428902320971090703239693491996277899" +
			"532332018406452247646396635593736700936921275809208629319872" +
			"7008292431243681"
		);
		final BigInteger z = new BigInteger(
			"427338526681239414707099486152541907807623930474842759553127" +
			"699575212802021361367225451651600353733949495680760238284875" +
			"258690199022379638588291839885522498545851997481849074579523" +
			"880422628363751913235562086585480775061024927773968205036369" +
			"669785002263076319003533000450157772067087172252728016627835" +
			"400463807389033342175518988780339070669313124967596962087173" +
			"533318107116757443584187074039849389081123568362582652760250" +
			"029401090870231288509578454981440888629750522601069337564316" +
			"940360631375375394366442662022050529545706707758321979377282" +
			"989361374561414204719371297211725179287931039547753581030226" +
			"7611143659071382"
		);

		BigInteger message = bruteForce(t, n).xor(z);
		System.out.println("brute force message: " + message);

		message = montgomery(t, n).xor(z);
		System.out.println("montgomery message: " + message);
	}

}
