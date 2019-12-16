namespace UnityPingAPIScript
{
	using System;
	class Program
	{
		static void Main(string[] args)
		{
			string destination1 = "google.com";         // Can resolve and ping
			string destination2 = "amazon.com";         // Can resolve and ping
			string destination3 = "microsoft.com";      // Can resolve, but can't ping
			string destination4 = "xyz.xxxyyyzzz";      // Test for an invalid host name
			string destination5 = string.Empty;         // Test for an empty string
			string destination6 = null;                 // Test for null value
			string destination7 = "outlook.com";        // Can resolve and ping

			PingDestination(destination1);
			PingDestination(destination2);
			PingDestination(destination3);
			PingDestination(destination4);
			PingDestination(destination5);
			PingDestination(destination6);
			PingDestination(destination7);
		}

		private static void PingDestination(string destination)
		{
			try
			{
				var ping = new Ping(destination);
				System.Console.Out.WriteLine(string.Format("Host: '{0}' Ping time: {1}\n", destination, ping.Time));
			}
			catch (ApplicationException ex)
			{
				System.Console.Out.WriteLine(string.Format("Host: '{0}' Ping thrown an exception: {1}.\n", destination, ex.Message));
			}
		}
	}
}
