import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Collections;

public class TestServer {

	public static final int DEFAULT_PORT = 5555;
	private static int clientNo = 0;
	
	private static void handleClient(final int clientNo, final Socket clientSocket) throws IOException {
		System.out.println("#" + clientNo + ": New connection from "
		  + clientSocket.getRemoteSocketAddress() + "...");

		PrintWriter out = null;
		BufferedReader in = null;
		try {
			out = new PrintWriter(clientSocket.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			String inputLine;
			while ((inputLine = in.readLine()) != null) {
				System.out.println("#" + clientNo + ">> " + inputLine);
				if (inputLine.equals("quit")) {
					break;
				}
			}
		} catch (IOException ex) {
			if (in != null) {
				in.close();
			}
			if (out != null) {
				out.close();
			}
			if (clientSocket != null) {
				clientSocket.close();
			}
		}

		System.out.println("#" + clientNo + ": Closing connection.");
	}

	private static void handleClientNewThread(final int clientNo, final Socket clientSocket) {
		(new Thread() {
			public void run() {
				try {
					handleClient(clientNo, clientSocket);
				} catch (IOException ex) {
					System.out.println(ex);
				}
			}
		}).start();
	}

	public static void main(String[] args) throws IOException {
		
		int portNumber = DEFAULT_PORT;

		if (args.length >= 1) {
			portNumber = Integer.parseInt(args[0]);
		}

		System.out.println("Available IP addresses:");
		for (NetworkInterface n : Collections.list(NetworkInterface.getNetworkInterfaces())) {
			for (InetAddress i : Collections.list(n.getInetAddresses())) {
				System.out.println("> " + i.getHostAddress());
			}
		}

		ServerSocket serverSocket = null;
		try {

			serverSocket = new ServerSocket(portNumber);
			System.out.println("Listening for connections on TCP port " + portNumber + "...");
			while (true) {
				Socket clientSocket = serverSocket.accept();
				clientNo++;
				handleClientNewThread(clientNo, clientSocket);
			}
		} catch (IOException ex) {
			if (serverSocket != null) {
				serverSocket.close();
			}
		}
	}
}


