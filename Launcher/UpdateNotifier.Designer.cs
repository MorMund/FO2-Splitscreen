namespace Launcher
{
    partial class UpdateNotifier
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.VersionCompare = new System.Windows.Forms.Label();
            this.Changelog = new System.Windows.Forms.RichTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.DownloadLink = new System.Windows.Forms.LinkLabel();
            this.OkButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(27, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(231, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "New Version available!";
            // 
            // VersionCompare
            // 
            this.VersionCompare.AutoSize = true;
            this.VersionCompare.Location = new System.Drawing.Point(32, 48);
            this.VersionCompare.Name = "VersionCompare";
            this.VersionCompare.Size = new System.Drawing.Size(154, 13);
            this.VersionCompare.TabIndex = 1;
            this.VersionCompare.Text = "Current Version : New Version :";
            // 
            // Changelog
            // 
            this.Changelog.BackColor = System.Drawing.Color.White;
            this.Changelog.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Changelog.Location = new System.Drawing.Point(32, 89);
            this.Changelog.Name = "Changelog";
            this.Changelog.ReadOnly = true;
            this.Changelog.Size = new System.Drawing.Size(218, 144);
            this.Changelog.TabIndex = 2;
            this.Changelog.Text = "";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(32, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Changelog :";
            // 
            // DownloadLink
            // 
            this.DownloadLink.AutoSize = true;
            this.DownloadLink.Location = new System.Drawing.Point(32, 239);
            this.DownloadLink.Name = "DownloadLink";
            this.DownloadLink.Size = new System.Drawing.Size(55, 13);
            this.DownloadLink.TabIndex = 4;
            this.DownloadLink.TabStop = true;
            this.DownloadLink.Text = "Download";
            this.DownloadLink.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.DownloadLink_LinkClicked);
            // 
            // OkButton
            // 
            this.OkButton.Location = new System.Drawing.Point(99, 261);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 5;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            this.OkButton.Click += new System.EventHandler(this.OkButton_Click);
            // 
            // UpdateNotifier
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 296);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.DownloadLink);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.Changelog);
            this.Controls.Add(this.VersionCompare);
            this.Controls.Add(this.label1);
            this.Name = "UpdateNotifier";
            this.Text = "New Version Available!";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label VersionCompare;
        private System.Windows.Forms.RichTextBox Changelog;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.LinkLabel DownloadLink;
        private System.Windows.Forms.Button OkButton;
    }
}