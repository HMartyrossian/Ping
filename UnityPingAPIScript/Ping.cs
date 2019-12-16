namespace UnityPingAPIScript
{
	using System;
	using System.Runtime.InteropServices;

	// Class implements current Unity Script API Ping interface: https://docs.unity3d.com/ScriptReference/Ping.html
	public class Ping
	{
		[DllImport("UnityPingAPI.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool PingImpl(string str);

		[DllImport("UnityPingAPI.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool SuccessImpl(string str);

		[DllImport("UnityPingAPI.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		[return: MarshalAs(UnmanagedType.I8)]
		public static extern Int64 TimeImpl(string str);

		public Ping(string destination)
		{
			if (!string.IsNullOrWhiteSpace(destination))
			{
				this.destination = destination;
				this.ipAddressResolved = PingImpl(this.destination);
			}
			else
			{
				throw new InvalidDestinationHostName();
			}
		}

		public bool IPAddressResolved
		{
			get
			{
				return this.ipAddressResolved;
			}
		}

		public bool IsDone
		{
			get
			{
				// Have to validate invariant:
				if (this.IPAddressResolved)
				{
					if (!this.isDone)
					{
						this.isDone = SuccessImpl(this.destination);
					}

					return this.isDone;
				}
				else
				{
					throw new IPAddressNotResolved();
				}
			}
		}

		public Int64 Time
		{
			get
			{
				// Have to validate invariant:
				if (this.IsDone)
				{
					if (this.time == 0)
					{
						this.time = TimeImpl(this.destination);
					}

					return this.time;
				}
				else
				{
					throw new PingUnsuccessful();
				}
			}
		}

		private string destination;

		private bool ipAddressResolved;
		private bool isDone;
		private Int64 time;
	}
}
