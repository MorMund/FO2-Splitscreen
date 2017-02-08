namespace Launcher
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Linq;
    using System.Threading;
    using System.Windows.Forms;

    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            try
            {
                if (args.Contains("-noConfig"))
                {
                    using (var s = File.OpenRead(Setup.settingsFile))
                    {
                        using (var settings = Settings.LoadSettings(s))
                        {
                            FlatOut2.CreateSplitScreenSession(settings).Wait();
                            Application.Exit();
                            return;
                        }
                    }
                }
                else
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.SetUnhandledExceptionMode(UnhandledExceptionMode.ThrowException);
                    Application.ThreadException += Application_ThreadException;
                    Application.Run(new Setup());
                }
            }
            catch (Exception e)
            {
                ShowFatalExceptionDialog(e);

            }
        }

        private static void Application_ThreadException(object sender, ThreadExceptionEventArgs e)
        {
            ShowFatalExceptionDialog(e.Exception);
        }

        private static void ShowFatalExceptionDialog(Exception e)
        {
            MessageBox.Show(
                $"Unhandled {e.ToString()} in\n" +
                $"{e.Source}@{e.TargetSite}\n" +
                e.StackTrace,
                "Unhandled exception",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            Application.Exit();
        }
    }
}
