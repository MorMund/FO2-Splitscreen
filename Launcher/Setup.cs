namespace Launcher
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows.Forms;
    using static Settings;

    public partial class Setup : Form
    {
        public const string settingsFile = "Splitscreen_Settings.xml";

        private Task gameLaunch = null;
        private List<RECT> resolutions = null;
        private List<GamePad> connectedGamedPads = null;
        private List<GamePad> availablePads = new List<GamePad>();
        private Settings settings = null;
        private bool suppressUpdate = false;
        public Setup()
        {
            InitializeComponent();
        }

        private void Setup_Load(object sender, EventArgs e)
        {
            var load = Task.Factory.StartNew(LoadSettings);
            load.ContinueWith(t =>
            {
                if (t.Exception != null)
                {
                    throw t.Exception.InnerException;
                }
                UpdateSettings(this.settings);
                EnterConfigTab();
                ConfigTabControl.Enabled = true;
#if DEBUG
                                InputAttachConsole.Enabled = true;
                                InputAttachConsole.Visible = true;
#endif
            },
            CancellationToken.None, TaskContinuationOptions.None, TaskScheduler.FromCurrentSynchronizationContext());
        }

        private void LoadSettings()
        {
            ValidateInstallation();

            if (settings != null)
            {
                throw new InvalidOperationException("Settings can only be loaded once at the start of the application.");
            }

            try
            {
                using (var s = File.OpenRead(settingsFile))
                {
                    settings = Settings.LoadSettings(s);
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine($"Loading settings failed with exception : \n{e.ToString()}");
            }

            if (this.settings == null)
            {
                this.settings = new Settings(1);
            }
        }

        private void ValidateInstallation()
        {
            bool isValid = true;
            if (!File.Exists("FlatOut2.exe"))
            {
                MessageBox.Show(
                    "FlatOut2 executable not found! Make sure that Launcher.exe and FlatOut2.dll are inside the FlatOut2 game folder.",
                    "Installation Error!",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                isValid = false;
            }

            if (!File.Exists("FlatOut2.dll"))
            {
                MessageBox.Show(
                   "FlatOut2.dll not found! Make sure that Launcher.exe and FlatOut2.dll are inside the FlatOut2 game folder.",
                   "Installation Error!",
                   MessageBoxButtons.OK,
                   MessageBoxIcon.Error);
                isValid = false;
            }

            if (!Directory.Exists("savegame") || !File.Exists("Savegame/options.cfg"))
            {
                MessageBox.Show(
                   "Game settings not found! Run FlatOut 2 normally once before using this launcher.",
                   "Installation Error!",
                   MessageBoxButtons.OK,
                   MessageBoxIcon.Error);
                isValid = false;
            }

            if (!isValid)
            {
                Application.Exit();
            }
        }

        private void ButtonLaunch_Click(object sender, EventArgs e)
        {
            if (gameLaunch != null)
            {
                return;
            }

            ButtonLaunch.Enabled = false;
            RECT selected = new RECT(0, 0, 640, 480);
            if (resolutions != null && InputResoSelect.Enabled)
            {
                selected = resolutions[InputResoSelect.SelectedIndex];
            }
            this.settings.SetDefaultWindowPos(Math.Max(selected.Width, 640), Math.Max(selected.Height, 480));

            gameLaunch = FlatOut2.CreateSplitScreenSession(this.settings);
            gameLaunch.ContinueWith(t =>
            {
                using (Stream s = File.Open(settingsFile, FileMode.Create))
                {
                    this.settings.SaveSettings(s);
                }
                Application.Exit();
            },
            TaskContinuationOptions.OnlyOnRanToCompletion);
            gameLaunch.ContinueWith(t =>
            {
                throw t.Exception.InnerException;
            },
            CancellationToken.None,
            TaskContinuationOptions.OnlyOnFaulted,
            TaskScheduler.FromCurrentSynchronizationContext());


        }

        private void InputInstanceCount_ValueChanged(object sender, EventArgs e)
        {
            Settings oldSettings = settings;
            settings = new Settings((int)InputInstanceCount.Value);
            UpdateSettings(oldSettings);
            if (oldSettings != null)
            {
                settings.Dispose();
            }
        }

        private void UpdateSettings(Settings oldSettings)
        {
            InputInstanceCount.Value = settings.InstanceCount;
            InputAttachConsole.Checked = settings.XMLAttachConsole[0];
            RECT res = (oldSettings.GetWindowPos().Equals(RECT.Zero) ? new RECT(0, 0, 640, 480) : oldSettings.GetWindowPos());
            settings.SetDefaultWindowPos(res.Width, res.Height);
        }

        private void ConfigTabControl_Enter(object sender, EventArgs e)
        {
            EnterConfigTab();
        }

        private void TabController_Enter(object sender, EventArgs e)
        {
            ControllerPlayerList.Enabled = false;
            ControllerOptionsPanel.Enabled = false;
            ControllerBTResetAll.Enabled = false;
            Task<List<GamePad>> getControllers = FlatOut2.GetGamePads();
            getControllers.ContinueWith(UpdateControllerList, CancellationToken.None, TaskContinuationOptions.None, TaskScheduler.FromCurrentSynchronizationContext());
            ControllerPlayerList.Clear();
            for (int player = 0; player < settings.InstanceCount; player++)
            {
                ControllerPlayerList.Items.Add($"Player {player + 1}");
            }
        }

        private void EnterConfigTab()
        {
            Task<List<RECT>> getResolutions = FlatOut2.GetResolutions();
            getResolutions.ContinueWith(UpdateResolutionList, CancellationToken.None, TaskContinuationOptions.None, TaskScheduler.FromCurrentSynchronizationContext());
        }

        private void UpdateResolutionList(Task<List<RECT>> getResolutions)
        {
            if (getResolutions.Exception != null)
            {
                throw getResolutions.Exception.InnerException;
            }

            resolutions = getResolutions.Result;
            InputResoSelect.Items.Clear();
            foreach (var reso in resolutions)
            {
                InputResoSelect.Items.Add($"{reso.Width}  x {reso.Height}");
            }

            if (InputResoSelect.SelectedIndex == -1 && resolutions.Count > 0)
            {
                InputResoSelect.SelectedIndex = 0;
                if (!settings.GetWindowPos().Equals(RECT.Zero))
                {
                    for (int i = 0; i < resolutions.Count; i++)
                    {
                        if (settings.GetWindowPos().Equals(resolutions[i]))
                        {
                            InputResoSelect.SelectedIndex = i;
                        }
                    }
                }
            }

            InputResoSelect.Enabled = true;
        }

        private void UpdateControllerList(Task<List<GamePad>> getControllers)
        {
            if (getControllers.Exception != null)
            {
                throw getControllers.Exception.InnerException;
            }

            connectedGamedPads = getControllers.Result;
            settings.ResetControllerSettings();

            InputControllerSelect.Items.Clear();
            foreach (var gamepad in connectedGamedPads)
            {
                InputControllerSelect.Items.Add(gamepad.ToString());
            }

            ControllerPlayerList.Enabled = true;
            ControllerBTResetAll.Enabled = true;
        }

        private void ControllerBTResetAll_Click(object sender, EventArgs e)
        {
            ControllerBTResetAll.Enabled = false;
            ControllerOptionsPanel.Enabled = false;
            settings.ResetControllerSettings();
            ControllerOptionsPanel.Enabled = true;
            ControllerBTResetAll.Enabled = true;
        }

        private void ControllerPlayerList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ControllerPlayerList.SelectedIndices.Count > 0)
            {
                UpdateControllerSelect();
            }
            else
            {
                ControllerOptionsPanel.Enabled = false;
            }
        }

        private void UpdateControllerSelect()
        {
            if (suppressUpdate)
            {
                suppressUpdate = false;
                return;
            }

            List<GamePad> usedPads = settings.GetUsedControllers(connectedGamedPads);
            GamePad instancePad = settings.GetInstanceController(ControllerPlayerList.SelectedIndices[0], connectedGamedPads);
            availablePads.Clear();
            InputControllerSelect.Items.Clear();
            InputControllerSelect.Text = string.Empty;
            InputControllerSelect.SelectedIndex = -1;
            foreach (var pad in connectedGamedPads)
            {
                if (!usedPads.Contains(pad) || pad.Equals(instancePad))
                {
                    availablePads.Add(pad);
                    InputControllerSelect.Items.Add(pad.ToString());

                    if (pad.Equals(instancePad))
                    {
                        suppressUpdate = true;
                        InputControllerSelect.SelectedIndex = InputControllerSelect.Items.Count - 1;
                    }
                }
            }

            ControllerOptionsPanel.Enabled = true;
        }

        private void InputControllerSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (InputControllerSelect.SelectedIndex != -1)
            {
                settings.SetInstanceController(
                    ControllerPlayerList.SelectedIndices[0],
                    availablePads[InputControllerSelect.SelectedIndex]);
                UpdateControllerSelect();
            }
        }

        private void InputAttachConsole_CheckedChanged(object sender, EventArgs e)
        {
            settings.AttachConsoleToAll(InputAttachConsole.Checked);
        }
    }
}
