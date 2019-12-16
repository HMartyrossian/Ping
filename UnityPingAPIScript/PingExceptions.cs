namespace UnityPingAPIScript
{
	using System;

	public class InvalidDestinationHostName : ApplicationException
	{
		private static readonly string InvalidDestinationHostNameMsg = "Invalid host name";

		public InvalidDestinationHostName() : base (InvalidDestinationHostNameMsg) {}
	}

	public class IPAddressNotResolved : ApplicationException
	{
		private static readonly string IPAddressNotResolvedMsg = "Could not find host";

		public IPAddressNotResolved() : base(IPAddressNotResolvedMsg) { }
	}

	public class PingUnsuccessful : ApplicationException
	{
		private static readonly string PingUnsuccessfulMsg = "Ping request timed out";

		public PingUnsuccessful() : base(PingUnsuccessfulMsg) {}
	}
}
