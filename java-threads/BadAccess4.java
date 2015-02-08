/*
 * Created on 2004-11-28
 *
 */

/**
 * @author Jakub Jarz±bek
 * @author changed by kubek2k
 */

import java.util.*;

/**
 * Uzytkownik pobierajacy rozne dane z komputera
 * Kazdy uzytkownik jest jednym watkiem
 */
class User4 extends Thread{
	private String name;
	private Computer4 comp;
	public User4( String n, Computer4 c ) {
		super( n );
		name = n;
		this.comp = c;
		start();
	}
	public void run() {
		String rdata;
		while (true) {
			System.out.println( "zapisywanie danych przez " + name );
		   comp.login(this);
		   yield();
			comp.writeData(name);
			comp.logout();
			System.out.println( "pobieranie danych przez " + name );
			comp.login(this);
			yield();
			rdata = comp.readData();
			comp.logout();
			if (!name.equals(rdata)) {
				System.out.println("Nieprawilowy dostep do zasobow:" );
				System.out.println("Nazwa uzytkownika: " + name + " pobrane dane:" + rdata );
			}
		}
	}
	public int hashCode() {
		return name.hashCode();
	}
}

/**
 * Komputer pozwala na zalogowanie sie dowolnemu uzytkownikowi i zapisanie lub
 * odczytanie tylko swoich danych. Zak³adamy, ¿e maksymalnie mo¿e byc zalogowany jeden u¿ytkownik
 */
class Computer4 {
	private User4 loggedUser;
	private HashMap data = new HashMap();
	public Computer4() {}

	public synchronized void login(User4 u) {
		while (loggedUser != null) 
			try { 
				wait();
			}catch ( InterruptedException e ) {
				throw new RuntimeException(e);
			}
		Thread.yield();	
		loggedUser = u;
	}

	public synchronized void logout() {
		loggedUser = null;
		notifyAll();
	}
			
	public void writeData(String d) {
		data.put(loggedUser, d);
	}

	public String readData() {
		return (String)data.get(loggedUser);
   }
}

public class BadAccess4 {
	public static void main( String[] args ) {
		Computer4 c = new Computer4();
		User4[] users = new User4[]	{ new User4( "user1", c ), 
			new User4( "user2", c ), new User4( "user3", c) };
		new Timer().schedule( new TimerTask() {
			public void run() {
				System.exit(0);
			}
		}, 2000 );
	}
}

