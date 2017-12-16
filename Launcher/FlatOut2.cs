namespace Launcher
{
    using System;
    using System.Collections.Generic;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Threading.Tasks;
    using static Settings;

    /// <summary>
    /// A class for managing game launches and querying system configuration. Provides a managed interface for methods of the <code>FlatOut.dll</code>.
    /// </summary>
    public class FlatOut2 : IDisposable
    {
        /// <summary>
        /// The maximum amount of instances that can run at once.
        /// </summary>
        public const int MaxInstanceCount = 8;
        private const int SharedSettingsSizeCount = 3;
        private const int DeviceInfoLength = 512;

        private Settings settings;

        private FlatOut2(Settings settings)
        {
            if (settings == null)
            {
                throw new ArgumentNullException();
            }
            else if (settings.InstanceCount < 1 || settings.InstanceCount > MaxInstanceCount)
            {
                throw new ArgumentOutOfRangeException();
            }

            this.settings = settings;
        }

        /// <summary>
        /// Asynchronously starts a new split-screen session with the specified settings.
        /// </summary>
        /// <param name="settings">The settings used to setup the session.</param>
        /// <returns>Returns a task that represents the session startup process.</returns>
        public static Task CreateSplitScreenSession(Settings settings)
        {
            FlatOut2 fo2 = new FlatOut2(settings);
            return Task.Factory.StartNew(() => { StartSplitScreenSession(fo2); });
        }

        /// <summary>
        /// Asynchronously retrieves a list of connected controllers.
        /// </summary>
        /// <returns>Returns a task representing the querying operation.</returns>
        public static Task<List<GamePad>> GetGamePads()
        {
            return Task.Factory.StartNew(() =>
            {
                List<GamePad> pads = new List<GamePad>();
                InitGamePadList(pads);
                return pads;
            });
        }

        /// <summary>
        /// Asynchronously retrieves a list of available resolution modes.
        /// </summary>
        /// <returns>Returns a task representing the querying operation</returns>
        public static Task<List<RECT>> GetResolutions()
        {
            return Task.Factory.StartNew(() =>
            {
                return new List<RECT>(new RECT[] { new RECT(0, 0, 1920 / 2, 1080 / 2) });
            });
        }

        [DllImport("FlatOut2.dll", EntryPoint = "CreateNewInstance", ExactSpelling = false, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int CreateNewInstance([MarshalAs(UnmanagedType.LPWStr)] string cmdLine);

        [DllImport("FlatOut2.dll", EntryPoint = "GetControllerGuids", ExactSpelling = false, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern bool GetControllerGuids([Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] ControllerInfo[] controllers, ref int deviceCount);

        [DllImport("FlatOut2.dll", EntryPoint = "GetResolutions", ExactSpelling = false, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern bool GetResolutions([Out, MarshalAs(UnmanagedType.LPArray)] RECT[] resolutions, ref int resoCount);

        private static void StartSplitScreenSession(FlatOut2 fo2)
        {
            using (fo2)
            {
                fo2.StartGame();
            }
        }

        private static void InitResolutionsList(List<RECT> resolutions)
        {
            if (resolutions == null)
            {
                throw new ArgumentNullException();
            }

            int resoCount = 20;
            int oldCount;
            RECT[] resos;
            while (true)
            {
                resos = new RECT[resoCount];
                oldCount = resoCount;
                if (!GetResolutions(resos, ref resoCount))
                {
                    throw new Exception();
                }
                else if (oldCount >= resoCount)
                {
                    break;
                }

                oldCount = resoCount;
            }

            resolutions.Clear();
            foreach (var reso in resos)
            {
                if (reso.Width != 0 && reso.Height != 0)
                {
                    resolutions.Add(reso);
                }
            }
        }

        private static void InitGamePadList(List<GamePad> gamePads)
        {
            if (gamePads == null)
            {
                throw new ArgumentNullException();
            }

            int deviceCount = 10;
            int oldCount;
            gamePads.Clear();
            ControllerInfo[] deviceBuffer;
            while (true)
            {
                oldCount = deviceCount;
                deviceBuffer = new ControllerInfo[deviceCount];
                if (!GetControllerGuids(deviceBuffer, ref deviceCount))
                {
                    throw new Exception();
                }
                else if (oldCount >= deviceCount)
                {
                    break;
                }
            }

            for (int i = 0; i < deviceCount; i++)
            {
                var device = deviceBuffer[i];
                gamePads.Add(new GamePad(device.Guid, new string(device.DeviceName), new string(device.InstanceName)));
            }
        }

        private void StartGame()
        {
            settings.SetupGameSettings();
            for (int i = 0; i < settings.InstanceCount; i++)
            {
                CreateNewInstance($"-i {i}");
            }

            settings.WaitForHostInstance();
        }

        public void Dispose()
        {
            ((IDisposable)settings).Dispose();
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private struct ControllerInfo
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 512)]
            public char[] DeviceName;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 512)]
            public char[] InstanceName;
            public Guid Guid;
        }
    }
}
