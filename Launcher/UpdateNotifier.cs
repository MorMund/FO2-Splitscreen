using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Launcher
{
    public partial class UpdateNotifier : Form
    {
        const string VerCompString = "Current Version : {0} New Version : {1}";

        private string downloadURI = "https://github.com/DeadlySurprise/FO2-Splitscreen/releases";

        public UpdateNotifier(string version, string[] changelog, string downloadLinkText, string downloadLinkURI)
        {
            InitializeComponent();
            VersionCompare.Text = string.Format(VerCompString, AutoUpdate.LauncherVersion, version);
            Changelog.Text = changelog.Select(s => "*" + s).Aggregate((a, b) => string.Concat(a, "\n", b));
            DownloadLink.Text = downloadLinkText;
        }

        private void DownloadLink_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start(downloadURI);
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
