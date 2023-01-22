namespace Launcher
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.IO.MemoryMappedFiles;
    using System.Net;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Text.RegularExpressions;
    using System.Threading;
    using System.Windows.Forms;
    using System.Xml.Serialization;

    /// <summary>
    /// Manages the game sessions settings and provides methods for saving/loading and pushing to the game instances. 
    /// </summary>
    [XmlType]
    public class Settings : IDisposable
    {
        #region Constants
        private const string DefaultVirtualNetworkAddress = "192.168.80.0";
        private const int GameConfigResolutionOffset = 0x78;
        private const int SharedSettingsSizeCount = 3;
        private const string SharedSettingsName = @"Local\FO2_SS_Shared";
        private const string LauncherWaitHandleName = @"Local\FO2_SS_Wait";
        public enum LogLevel : int { Disabled, Error, Warning, Info, Debug, Trace }
        #endregion

        #region Fields
        private bool isInitialized;
        private int instanceCount;
        private RECT[] windowPos;
        private Guid[] controllers;
        private IPAddress virtAddressRange;
        private RECT gameResolution;
        private MemoryMappedFile settingsMem;
        private int settingsSize = -1;
        private WaitHandle waitHandle;
        private bool useInputEmulation;
        private bool skipIntros = true;
        private bool useBackgroundAudio = true;
        #endregion

        #region Constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="Settings" /> class. Default constructor for creating default settings.
        /// </summary>
        /// <param name="instanceCount">The number of game instances used for this session.</param>
        public Settings(int instanceCount)
        {
            InstanceCount = instanceCount;
            windowPos = new RECT[instanceCount];
            ResetControllerSettings();
            virtAddressRange = IPAddress.Parse(DefaultVirtualNetworkAddress);
            isInitialized = true;
        }

        /// <summary>
        /// Prevents a default instance of the <see cref="Settings" /> class from being created.
        /// </summary>
        private Settings()
        {
        }
        #endregion

        #region Public properties
        /// <summary>
        /// Gets or sets the number of game instances in this session. (Setting this property is only for XML loading.)
        /// </summary>
        [XmlElement(ElementName = "InstanceCount")]
        public int InstanceCount
        {
            get
            {
                return instanceCount;
            }

            set
            {
                if (isInitialized)
                {
                    throw new ArgumentException("Instance count can only be set through the constructor!");
                }
                else if (value < 1 || value > FlatOut2.MaxInstanceCount)
                {
                    throw new ArgumentOutOfRangeException();
                }
                else
                {
                    instanceCount = value;
                    isInitialized = true;
                }
            }
        }

        /// <summary>
        /// Gets or sets the port used by the host instance to communicated with its virtual clients.
        /// </summary>
        [XmlElement(ElementName = "VirtualHostPort")]
        public ushort VirtualHostPort { get; set; } = 23758;

        /// <summary>
        /// Gets or sets the offset the virtual address range begins at. E.g. address range 192.168.80, offset 10, 8 game instances => range 192.168.80.10 - 192.168.80.18.
        /// </summary>
        [XmlElement(ElementName = "VirtualAddressOffset")]
        public byte VirtualAddressOffset { get; set; } = 1;

        /// <summary>
        /// Gets or sets the verbosity of the log output written to the console window.
        /// </summary>
        [XmlElement(ElementName = "ConsoleVerbosity")]
        public LogLevel ConsoleVerbosity { get; set; }


        /// <summary>
        /// Gets or sets the verbosity of the log output written to the log file.
        /// </summary>
        [XmlElement(ElementName = "LogFileVerbosity")]
        public LogLevel LogFileVerbosity { get; set; }

        /// <summary>
        /// Only for XML serialization!
        /// </summary>
        [XmlElement(ElementName = "WindowPos")]
        public string[] XmlWindowPos
        {
            get
            {
                string[] ret = new string[windowPos.Length];
                for (int i = 0; i < windowPos.Length; i++)
                {
                    ret[i] = windowPos[i].ToString();
                }

                return ret;
            }

            set
            {
                if (value.Length > FlatOut2.MaxInstanceCount)
                {
                    throw new ArgumentOutOfRangeException();
                }

                windowPos = new RECT[value.Length];
                for (int i = 0; i < value.Length; i++)
                {
                    windowPos[i] = RECT.Parse(value[i]);
                }
            }
        }

        /// <summary>
        /// Only for XML serialization!
        /// </summary>
        [XmlElement(ElementName = "Controllers")]
        public string[] XmlControllers
        {
            get
            {
                string[] ret = new string[controllers.Length];
                for (int i = 0; i < controllers.Length; i++)
                {
                    ret[i] = controllers[i].ToString();
                }

                return ret;
            }

            set
            {
                if (value.Length > FlatOut2.MaxInstanceCount)
                {
                    throw new ArgumentOutOfRangeException();
                }

                controllers = new Guid[value.Length];
                for (int i = 0; i < value.Length; i++)
                {
                    controllers[i] = Guid.Parse(value[i]);
                }
            }
        }

        /// <summary>
        /// Only for XML serialization!
        /// </summary>
        [XmlElement(ElementName = "VirtualAddressRange")]
        public string XmlVirtualAddressRange
        {
            get
            {
                return virtAddressRange.ToString();
            }

            set
            {
                virtAddressRange = IPAddress.Parse(value);
            }
        }

        /// <summary>
        /// Gets or sets the address range used for the virtual clients. Only <code>IPv4</code> address are allowed and only the first 3 octets (network part) are used.
        /// The last octet (host part) is defined by <see cref="VirtualAddressOffset"/> and the virtual clients instance index.
        /// </summary>
        [XmlIgnore]
        public IPAddress VirtualAddressRange
        {
            get
            {
                return virtAddressRange;
            }

            set
            {
                if (value.AddressFamily != System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    throw new ArgumentException();
                }

                virtAddressRange = value;
            }
        }

        /// <summary>
        /// Gets or sets if the game should use gamepad to keyboard emulation. (CURRENTLY BUGGY. KEEP DISABLED FOR RELEASES.)
        /// </summary>
        [XmlElement(ElementName = "UseInputEmulation")]
        public bool UseInputEmulation
        {
            get
            {
                return useInputEmulation;
            }

            set
            {
                useInputEmulation = value;
            }
        }

        /// <summary>
        /// Gets or sets if the intro videos should be skipped
        /// </summary>
        [XmlElement(ElementName = "SkipIntros")]
        public bool SkipIntros
        {
            get
            {
                return skipIntros;
            }

            set
            {
                skipIntros = value;
            }
        }

        /// <summary>
        /// Gets or sets if the intro videos should be skipped
        /// </summary>
        [XmlElement(ElementName = "BackgroundAudio")]
        public bool UseBackgroundAudio
        {
            get
            {
                return useBackgroundAudio;
            }

            set
            {
                useBackgroundAudio = value;
            }
        }
        #endregion

        #region Public methods
        /// <summary>
        /// Loads session settings from a XML stream.
        /// </summary>
        /// <param name="s">The stream in XML format to read from.</param>
        /// <returns>Return the settings loaded from the XML stream.</returns>
        public static Settings LoadSettings(Stream s)
        {
            XmlSerializer xml = new XmlSerializer(typeof(Settings));
            return xml.Deserialize(s) as Settings;
        }

        /// <summary>
        /// Sets up the games settings files and pushes session settings to instance shared memory.
        /// </summary>
        public void SetupGameSettings()
        {
            settingsSize = GetSharedSettingsSize();
            try
            {
                SetupInstances();
            }
            catch (IOException io)
            {
                throw new Exception("Could not setup instance settings. Make sure no instances of the game are running before launching!", io);
            }

            if (gameResolution.Width > 0 && gameResolution.Height > 0)
            {
                using (BinaryWriter cfg = new BinaryWriter(File.OpenWrite("Savegame/device.cfg")))
                {
                    cfg.Seek(GameConfigResolutionOffset, SeekOrigin.Begin);
                    cfg.Write((uint)gameResolution.Width);
                    cfg.Write((uint)gameResolution.Height);
                }
            }

            // Workaround for game crashing on map load or entering options menu
            string[] options = File.ReadAllLines("Savegame/options.cfg");
            for (int i = 0; i < options.Length; i++)
            {
                if (options[i].Contains("Settings.Control.Controller ="))
                {
                    options[i] = "Settings.Control.Controller = 0 			--[0 .. 2]";
                }
                else if (options[i].Contains("Settings.Control.ControllerGuid ="))
                {
                    options[i] = "Settings.Control.ControllerGuid = \"00000000000000000000000000000000\"";
                }
            }
        }

        /// <summary>
        /// Sets all instance windows size and orders them from left to right (with wrapping) on available screen space.
        /// </summary>
        /// <param name="windowWidth">Width of a one instance window.</param>
        /// <param name="windowHeight">Height of a one instance window.</param>
        public void SetDefaultWindowPos(int windowWidth, int windowHeight)
        {
            gameResolution = new RECT(0, 0, windowWidth, windowHeight);
            int screenX = SystemInformation.VirtualScreen.X;
            int screenY = SystemInformation.VirtualScreen.Y;
            int screenW = SystemInformation.VirtualScreen.Width;
            int windowX = screenX;
            int windowY = screenY;
            for (int i = 0; i < instanceCount; i++)
            {
                windowPos[i] = new RECT(windowX, windowY, windowWidth, windowHeight);
                windowX += windowWidth;
                if (windowX + windowWidth > screenX + screenW)
                {
                    windowX = screenX;
                    windowY += windowHeight;
                }
            }
        }

        /// <summary>
        /// Waits for the primary(host) game instance to signal that it has finished loading. This also signals at what point the shared settings memory is safe to close on this process.
        /// </summary>
        public void WaitForHostInstance()
        {
            waitHandle.WaitOne();
        }

        /// <summary>
        /// Resets all player's assigned controllers to none.
        /// </summary>
        public void ResetControllerSettings()
        {
            controllers = new Guid[instanceCount];
        }

        /// <summary>
        /// Assigns a controller to a game instance.
        /// </summary>
        /// <param name="instanceIndex">The index of game instance the controller will be assigned to.</param>
        /// <param name="controller">The controller that will be assigned.</param>
        public void SetInstanceController(int instanceIndex, GamePad controller)
        {
            if (controller == null)
            {
                throw new ArgumentNullException();
            }
            else if (instanceIndex < 0 || instanceIndex >= instanceCount)
            {
                throw new ArgumentOutOfRangeException();
            }

            controllers[instanceIndex] = controller.Guid;
        }

        /// <summary>
        /// Returns the current assigned controller of a game instance.
        /// </summary>
        /// <param name="instanceIndex">The index of the instance to query.</param>
        /// <param name="availableControllers">A list of all connected controllers.</param>
        /// <returns>Returns the controller assigned to this instance or null if there isn't one.</returns>
        public GamePad GetInstanceController(int instanceIndex, List<GamePad> availableControllers)
        {
            foreach (var controller in availableControllers)
            {
                if (controller.Guid == controllers[instanceIndex])
                {
                    return controller;
                }
            }

            return null;
        }

        /// <summary>
        /// Gets a list of all controllers currently assigned to a game instance.
        /// </summary>
        /// <param name="availableControllers">A list of all connected controllers.</param>
        /// <returns>Return a list of all controllers currently assigned to a game instance.</returns>
        public List<GamePad> GetUsedControllers(List<GamePad> availableControllers)
        {
            List<GamePad> used = new List<GamePad>();
            foreach (var controller in availableControllers)
            {
                foreach (var guid in controllers)
                {
                    if (controller.Guid == guid)
                    {
                        used.Add(controller);
                    }
                }
            }

            return used;
        }

        /// <summary>
        /// Saves the currently selected session settings in XML format.
        /// </summary>
        /// <param name="s">The stream to write the settings in.</param>
        public void SaveSettings(Stream s)
        {
            XmlSerializer xml = new XmlSerializer(typeof(Settings));
            xml.Serialize(s, this);
        }

        /// <summary>
        /// Gets the game window position of an instance.
        /// </summary>
        /// <param name="instanceIndex">The index of the instance. Default is 0 i.e. the host instance.</param>
        /// <returns>Returns the game window position of an instance.</returns>
        public RECT GetWindowPos(int instanceIndex = 0)
        {
            if (instanceIndex < 0 || instanceIndex >= instanceCount)
            {
                throw new ArgumentOutOfRangeException();
            }

            return windowPos[instanceIndex];
        }
        #endregion

        [DllImport("FlatOut2.dll", EntryPoint = "GetSharedSettingsSize", ExactSpelling = false, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern bool GetSharedSettingsSize([MarshalAs(UnmanagedType.LPArray, SizeConst = SharedSettingsSizeCount)] int[] sizes, int sizeCount);

        #region Private methods
        private int GetSharedSettingsSize()
        {
            int a = Marshal.SizeOf<RECT>();
            int[] sizes = new int[SharedSettingsSizeCount];
            bool success = GetSharedSettingsSize(sizes, sizes.Length);
            if (!success)
            {
                throw new InvalidComObjectException("The version of the Flatout2.dll does not match the launchers version!");
            }

            if (Marshal.SizeOf<SET_GlobalSettings>() != sizes[1])
            {
                StringBuilder str = new StringBuilder("The version of the Flatout2.dll does not match the launchers version!\n");
                str.Append($"Size of GlobalSettings structure differ : (Native : {sizes[1]} | Managed : {Marshal.SizeOf<SET_GlobalSettings>()})");
                throw new InvalidComObjectException(str.ToString());
            }

            int instanceSettingsSize = sizes[2] * FlatOut2.MaxInstanceCount;
            if (Marshal.SizeOf<SET_SharedSettings>() + instanceSettingsSize != sizes[0])
            {
                StringBuilder str = new StringBuilder("The version of the Flatout2.dll does not match the launchers version!\n");
                str.Append($"Size of SharedSettings structure differ : (Native : {sizes[0]} | Managed : {Marshal.SizeOf<SET_SharedSettings>()})");
                throw new InvalidComObjectException(str.ToString());
            }

            return sizes[0];
        }

        private void SetupInstances()
        {
            bool createNew;
            waitHandle = new EventWaitHandle(false, EventResetMode.ManualReset, LauncherWaitHandleName, out createNew);
            if (!createNew)
            {
                throw new InvalidOperationException();
            }

            SET_SharedSettings shared = new SET_SharedSettings();
            shared.VirtClientPorts = new ushort[FlatOut2.MaxInstanceCount];
            SET_GlobalSettings global = new SET_GlobalSettings();
            global.InstanceCount = instanceCount;
            global.UseInputEmulation = useInputEmulation;
            global.SkipIntros = skipIntros;
            global.useBackgroundAudio = useBackgroundAudio;
            global.LogFileVerbosity = LogFileVerbosity;
            global.ConsoleVerbosity = ConsoleVerbosity;
            global.WindowPos = new RECT[FlatOut2.MaxInstanceCount];
            global.Controller = new Guid[FlatOut2.MaxInstanceCount];
            global.VirtHostPort = VirtualHostPort;
            global.VirtAddressOffset = VirtualAddressOffset;
            global.VirtAddressRange = new char[16];
            virtAddressRange.ToString().ToCharArray().CopyTo(global.VirtAddressRange, 0);
            for (int i = 0; i < instanceCount; i++)
            {
                global.WindowPos[i] = windowPos[i];
                global.Controller[i] = controllers[i];
            }

            shared.Globals = global;

            if (settingsMem == null)
            {
                settingsMem = MemoryMappedFile.CreateNew(
                    SharedSettingsName,
                    settingsSize,
                    MemoryMappedFileAccess.ReadWriteExecute,
                    MemoryMappedFileOptions.None,
                    null,
                    HandleInheritability.None);
            }

            using (MemoryMappedViewStream stream = settingsMem.CreateViewStream())
            {
                int length = Marshal.SizeOf<SET_SharedSettings>();
                IntPtr ptr = Marshal.AllocHGlobal(length);
                byte[] structBuf = new byte[length];

                Marshal.StructureToPtr(shared, ptr, true);
                Marshal.Copy(ptr, structBuf, 0, length);
                stream.Write(structBuf, 0, length);
                Marshal.FreeHGlobal(ptr);
            }
        }
        #endregion

        #region Structures
        /// <summary>
        /// A structure representing the WINAPI RECT structure for rectangles. Provides methods for converting from/to strings.
        /// </summary>
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct RECT
        {
            public static readonly RECT Zero = new RECT(0, 0, 0, 0);
            public readonly int X, Y, Width, Height;

            public RECT(int x, int y, int width, int height)
            {
                X = x;
                Y = y;
                Width = width;
                Height = height;
            }

            /// <summary>
            /// Converts a string of the format {x,y,width,height} to a RECT structure.
            /// </summary>
            /// <param name="input">A string of the format {x,y,width,height}.</param>
            /// <returns>Returns a RECT converted from the input string.</returns>
            public static RECT Parse(string input)
            {
                if (!Regex.IsMatch(input, "{x:-*\\d+ y:-*\\d+ width:-*\\d+ height:-*\\d+}"))
                {
                    throw new FormatException();
                }

                input = input.Substring(1, input.Length - 2); // Remove brackets
                string[] values = input.Split(':', ' ');
                RECT ret = new RECT(
                    int.Parse(values[1]),
                    int.Parse(values[3]),
                    int.Parse(values[5]),
                    int.Parse(values[7]));
                return ret;
            }

            /// <summary>
            /// Converts the RECT to a string of the format {x,y,width,height}.
            /// </summary>
            /// <returns>Return a string in the format {x,y,width,height}</returns>
            public override string ToString()
            {
                return $"{{x:{X} y:{Y} width:{Width} height:{Height}}}";
            }

            /// <inheritdoc />
            public override int GetHashCode()
            {
                return X ^ -Y ^ Width ^ -Height;
            }

            /// <inheritdoc />
            public override bool Equals(object obj)
            {
                if (obj is RECT)
                {
                    RECT r = (RECT)obj;
                    if (r.X == X && r.Y == Y &&
                        r.Width == Width && r.Height == Height)
                    {
                        return true;
                    }
                }

                return false;
            }
        }

        private struct SET_GlobalSettings
        {
            public int InstanceCount;
            public bool UseInputEmulation;
            public bool SkipIntros;
            public bool useBackgroundAudio;
            public ushort VirtHostPort;
            public byte VirtAddressOffset;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public char[] VirtAddressRange;
            public LogLevel LogFileVerbosity;
            public LogLevel ConsoleVerbosity;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = FlatOut2.MaxInstanceCount)]
            public Guid[] Controller;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = FlatOut2.MaxInstanceCount)]
            public RECT[] WindowPos;
        }

        private struct SET_SharedSettings
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = FlatOut2.MaxInstanceCount)]
            public ushort[] VirtClientPorts;
            public SET_GlobalSettings Globals;
        }
        #endregion

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        /// <inheritdoc />
        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    settingsMem?.Dispose();
                    waitHandle?.Dispose();
                }

                disposedValue = true;
            }
        }

        /// <summary>
        /// Disposes this settings object.
        /// </summary>
        //// This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
        }
        #endregion
    }
}
