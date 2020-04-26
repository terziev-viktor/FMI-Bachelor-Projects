import java.io.*;
import java.net.InetAddress;
import java.util.Vector;

public class BlacklistClient {
    public static void reverse(byte[] bytes) {
        for(int i = 0; i < bytes.length / 2; ++i) {
            byte tmp = bytes[i];
            bytes[i] = bytes[bytes.length - i - 1];
            bytes[bytes.length - i - 1] = tmp;
        }
    }

    public static void main(String args[]) throws IOException, InterruptedException {
        for (String ip: args) {
            InetAddress iprev = InetAddress.getByName(ip);
            byte[] bytes = iprev.getAddress();
            reverse(bytes);
            iprev = InetAddress.getByAddress(bytes);

            try {
                InetAddress[] addresses = InetAddress.getAllByName(iprev.toString().substring(1) + ".zen.spamhaus.org");
                for (var address : addresses) {
                    switch (address.getCanonicalHostName()) {
                        case "127.0.0.2": {
                            System.out.println("The IP address:" + ip + " is found in the following Spamhaus public IP zone: '127.0.0.2 - SBL - Spamhaus SBL Data'");
                            break;
                        }
                        case "127.0.0.3": {
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '127.0.0.3 - SBL - Spamhaus SBL CSS Data'");
                            break;
                        }
                        case "127.0.0.4": {
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '127.0.0.4 - XBL - CBL Data'");
                            break;
                        }
                        case "127.0.0.9": {
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '127.0.0.9 - SBL - Spamhaus DROP/EDROP Data'");
                            break;
                        }
                        case "127.0.0.10": {
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '127.0.0.10 - PBL - ISP Maintained'");
                            break;
                        }
                        case "127.0.0.11": {
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '127.0.0.11 - PBL - Spamhaus Maintained'");
                            break;
                        }
                        default:
                            System.out.println("The IP address: " + ip + " is found in the following Spamhaus public IP zone: '" + address.getCanonicalHostName() + "'");
                    }
                }

            } catch (java.net.UnknownHostException e) {
                System.out.println("The IP address: " + ip + "  is NOT found in the Spamhaus blacklists.");
            }
        }
    }
}
