using System;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Management;

namespace client_service_windows
{
    public partial class Service1 : ServiceBase
    {
        public EventLog log = new EventLog();

        public Service1()
        {
            InitializeComponent();

            if (!EventLog.SourceExists("ClientServiceWindows"))
                EventLog.CreateEventSource("ClientServiceWindows", "Application");

            log.Source = "ClientServiceWindows";
            log.WriteEntry("Service constructed.");
        }

        protected override void OnStart(string[] args)
        {
            notifyLogon();
        }

        protected override void OnStop()
        {
            notifyLogoff();
        }

        protected override void OnSessionChange(SessionChangeDescription changeDescription)
        {
            switch (changeDescription.Reason)
            {
                case SessionChangeReason.ConsoleConnect:
                case SessionChangeReason.ConsoleDisconnect:
                case SessionChangeReason.RemoteConnect:
                case SessionChangeReason.RemoteDisconnect:
                case SessionChangeReason.SessionUnlock:
                    break;
                case SessionChangeReason.SessionLogon:
                    notifyLogon();
                    break;
                case SessionChangeReason.SessionLock:
                case SessionChangeReason.SessionLogoff:
                    notifyLogoff();
                    break;
                case SessionChangeReason.SessionRemoteControl:
                default:
                    break;
            }
        }

        public static byte[] WrapString(string message)
        {
            byte[] _message = Encoding.ASCII.GetBytes(message);

            // Get the length prefix for the message
            byte[] lengthPrefix = BitConverter.GetBytes(_message.Length);

            // convert to big-endian
            if (BitConverter.IsLittleEndian)
                Array.Reverse(lengthPrefix);

            // Concatenate the length prefix and the message
            byte[] ret = new byte[lengthPrefix.Length + _message.Length];
            lengthPrefix.CopyTo(ret, 0);
            _message.CopyTo(ret, lengthPrefix.Length);

            return ret;
        }

        public static byte[] Combine(byte[] first, byte[] second, byte[] third, byte[] fourth)
        {
            byte[] bytes = new byte[first.Length + second.Length + third.Length + fourth.Length];
            Buffer.BlockCopy(first, 0, bytes, 0, first.Length);
            Buffer.BlockCopy(second, 0, bytes, first.Length, second.Length);
            Buffer.BlockCopy(third, 0, bytes, first.Length + second.Length, third.Length);
            Buffer.BlockCopy(fourth, 0, bytes, first.Length + second.Length + third.Length, fourth.Length);
            return bytes;
        }

        public void notifyLogon()
        {
            log.WriteEntry("Notifying server of logon.");

            Socket soc = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1234);

            int unixTimestamp = (int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;

            ManagementObjectSearcher searcher = new ManagementObjectSearcher("SELECT UserName FROM Win32_ComputerSystem");
            ManagementObjectCollection collection = searcher.Get();
            string username = (string)collection.Cast<ManagementBaseObject>().First()["UserName"];

            byte[] actualMessage = Combine(WrapString("reportUserLogin"), WrapString(username), WrapString(unixTimestamp.ToString()), WrapString(unixTimestamp.ToString()));

            log.WriteEntry("Sending data to server: " + Encoding.ASCII.GetString(actualMessage) + " (" + actualMessage.Length + " bytes)");

            soc.SendTo(actualMessage, endpoint);
        }

        public void notifyLogoff()
        {
            log.WriteEntry("Notifying server of logon.");

            Socket soc = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1234);

            int unixTimestamp = (int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;

            ManagementObjectSearcher searcher = new ManagementObjectSearcher("SELECT UserName FROM Win32_ComputerSystem");
            ManagementObjectCollection collection = searcher.Get();
            string username = (string)collection.Cast<ManagementBaseObject>().First()["UserName"];

            byte[] actualMessage = Combine(WrapString("reportUserLogoff"), WrapString(username), WrapString(unixTimestamp.ToString()), WrapString(unixTimestamp.ToString()));

            log.WriteEntry("Sending data to server: " + Encoding.ASCII.GetString(actualMessage) + " (" + actualMessage.Length + " bytes)");

            soc.SendTo(actualMessage, endpoint);
        }
    }
}
