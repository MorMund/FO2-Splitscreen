﻿namespace Launcher
{
    partial class Setup
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Setup));
            this.ConfigTabControl = new System.Windows.Forms.TabControl();
            this.TabGeneralSettings = new System.Windows.Forms.TabPage();
            this.InputBackgroundSound = new System.Windows.Forms.CheckBox();
            this.InputSkipIntros = new System.Windows.Forms.CheckBox();
            this.InputResoSelect = new System.Windows.Forms.ComboBox();
            this.InputInstanceCount = new System.Windows.Forms.NumericUpDown();
            this.LabelResolution = new System.Windows.Forms.Label();
            this.LabelInstances = new System.Windows.Forms.Label();
            this.TabController = new System.Windows.Forms.TabPage();
            this.ControllerOptionsPanel = new System.Windows.Forms.Panel();
            this.LabelSelectController = new System.Windows.Forms.Label();
            this.InputControllerSelect = new System.Windows.Forms.ComboBox();
            this.ControllerBTResetAll = new System.Windows.Forms.Button();
            this.ControllerPlayerList = new System.Windows.Forms.ListView();
            this.TabDebugging = new System.Windows.Forms.TabPage();
            this.ConsoleVerbositySelect = new System.Windows.Forms.ComboBox();
            this.LabelConsoleVerbosity = new System.Windows.Forms.Label();
            this.LogFileVerbositySelect = new System.Windows.Forms.ComboBox();
            this.LabelDebugFileVerb = new System.Windows.Forms.Label();
            this.ButtonLaunch = new System.Windows.Forms.Button();
            this.NewVerLink = new System.Windows.Forms.LinkLabel();
            this.ConfigTabControl.SuspendLayout();
            this.TabGeneralSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InputInstanceCount)).BeginInit();
            this.TabController.SuspendLayout();
            this.ControllerOptionsPanel.SuspendLayout();
            this.TabDebugging.SuspendLayout();
            this.SuspendLayout();
            // 
            // ConfigTabControl
            // 
            this.ConfigTabControl.Controls.Add(this.TabGeneralSettings);
            this.ConfigTabControl.Controls.Add(this.TabController);
            this.ConfigTabControl.Controls.Add(this.TabDebugging);
            resources.ApplyResources(this.ConfigTabControl, "ConfigTabControl");
            this.ConfigTabControl.Name = "ConfigTabControl";
            this.ConfigTabControl.SelectedIndex = 0;
            this.ConfigTabControl.Enter += new System.EventHandler(this.ConfigTabControl_Enter);
            // 
            // TabGeneralSettings
            // 
            this.TabGeneralSettings.Controls.Add(this.InputBackgroundSound);
            this.TabGeneralSettings.Controls.Add(this.InputSkipIntros);
            this.TabGeneralSettings.Controls.Add(this.InputResoSelect);
            this.TabGeneralSettings.Controls.Add(this.InputInstanceCount);
            this.TabGeneralSettings.Controls.Add(this.LabelResolution);
            this.TabGeneralSettings.Controls.Add(this.LabelInstances);
            resources.ApplyResources(this.TabGeneralSettings, "TabGeneralSettings");
            this.TabGeneralSettings.Name = "TabGeneralSettings";
            this.TabGeneralSettings.UseVisualStyleBackColor = true;
            // 
            // InputBackgroundSound
            // 
            resources.ApplyResources(this.InputBackgroundSound, "InputBackgroundSound");
            this.InputBackgroundSound.Checked = true;
            this.InputBackgroundSound.CheckState = System.Windows.Forms.CheckState.Checked;
            this.InputBackgroundSound.Name = "InputBackgroundSound";
            this.InputBackgroundSound.UseVisualStyleBackColor = true;
            this.InputBackgroundSound.CheckedChanged += new System.EventHandler(this.InputBackgroundSound_CheckedChanged);
            // 
            // InputSkipIntros
            // 
            resources.ApplyResources(this.InputSkipIntros, "InputSkipIntros");
            this.InputSkipIntros.Checked = true;
            this.InputSkipIntros.CheckState = System.Windows.Forms.CheckState.Checked;
            this.InputSkipIntros.Name = "InputSkipIntros";
            this.InputSkipIntros.UseVisualStyleBackColor = true;
            this.InputSkipIntros.CheckedChanged += new System.EventHandler(this.InputSkipIntros_CheckedChanged);
            // 
            // InputResoSelect
            // 
            resources.ApplyResources(this.InputResoSelect, "InputResoSelect");
            this.InputResoSelect.FormattingEnabled = true;
            this.InputResoSelect.Name = "InputResoSelect";
            // 
            // InputInstanceCount
            // 
            resources.ApplyResources(this.InputInstanceCount, "InputInstanceCount");
            this.InputInstanceCount.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.InputInstanceCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.InputInstanceCount.Name = "InputInstanceCount";
            this.InputInstanceCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.InputInstanceCount.ValueChanged += new System.EventHandler(this.InputInstanceCount_ValueChanged);
            // 
            // LabelResolution
            // 
            resources.ApplyResources(this.LabelResolution, "LabelResolution");
            this.LabelResolution.Name = "LabelResolution";
            // 
            // LabelInstances
            // 
            resources.ApplyResources(this.LabelInstances, "LabelInstances");
            this.LabelInstances.Name = "LabelInstances";
            // 
            // TabController
            // 
            this.TabController.Controls.Add(this.ControllerOptionsPanel);
            this.TabController.Controls.Add(this.ControllerBTResetAll);
            this.TabController.Controls.Add(this.ControllerPlayerList);
            resources.ApplyResources(this.TabController, "TabController");
            this.TabController.Name = "TabController";
            this.TabController.UseVisualStyleBackColor = true;
            this.TabController.Enter += new System.EventHandler(this.TabController_Enter);
            // 
            // ControllerOptionsPanel
            // 
            this.ControllerOptionsPanel.Controls.Add(this.LabelSelectController);
            this.ControllerOptionsPanel.Controls.Add(this.InputControllerSelect);
            resources.ApplyResources(this.ControllerOptionsPanel, "ControllerOptionsPanel");
            this.ControllerOptionsPanel.Name = "ControllerOptionsPanel";
            // 
            // LabelSelectController
            // 
            resources.ApplyResources(this.LabelSelectController, "LabelSelectController");
            this.LabelSelectController.Name = "LabelSelectController";
            // 
            // InputControllerSelect
            // 
            this.InputControllerSelect.FormattingEnabled = true;
            resources.ApplyResources(this.InputControllerSelect, "InputControllerSelect");
            this.InputControllerSelect.Name = "InputControllerSelect";
            this.InputControllerSelect.SelectedIndexChanged += new System.EventHandler(this.InputControllerSelect_SelectedIndexChanged);
            // 
            // ControllerBTResetAll
            // 
            resources.ApplyResources(this.ControllerBTResetAll, "ControllerBTResetAll");
            this.ControllerBTResetAll.Name = "ControllerBTResetAll";
            this.ControllerBTResetAll.UseVisualStyleBackColor = true;
            this.ControllerBTResetAll.Click += new System.EventHandler(this.ControllerBTResetAll_Click);
            // 
            // ControllerPlayerList
            // 
            resources.ApplyResources(this.ControllerPlayerList, "ControllerPlayerList");
            this.ControllerPlayerList.HideSelection = false;
            this.ControllerPlayerList.MultiSelect = false;
            this.ControllerPlayerList.Name = "ControllerPlayerList";
            this.ControllerPlayerList.UseCompatibleStateImageBehavior = false;
            this.ControllerPlayerList.SelectedIndexChanged += new System.EventHandler(this.ControllerPlayerList_SelectedIndexChanged);
            // 
            // TabDebugging
            // 
            this.TabDebugging.Controls.Add(this.ConsoleVerbositySelect);
            this.TabDebugging.Controls.Add(this.LabelConsoleVerbosity);
            this.TabDebugging.Controls.Add(this.LogFileVerbositySelect);
            this.TabDebugging.Controls.Add(this.LabelDebugFileVerb);
            resources.ApplyResources(this.TabDebugging, "TabDebugging");
            this.TabDebugging.Name = "TabDebugging";
            this.TabDebugging.UseVisualStyleBackColor = true;
            // 
            // ConsoleVerbositySelect
            // 
            this.ConsoleVerbositySelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ConsoleVerbositySelect.FormattingEnabled = true;
            this.ConsoleVerbositySelect.Items.AddRange(new object[] {
            resources.GetString("ConsoleVerbositySelect.Items"),
            resources.GetString("ConsoleVerbositySelect.Items1"),
            resources.GetString("ConsoleVerbositySelect.Items2"),
            resources.GetString("ConsoleVerbositySelect.Items3"),
            resources.GetString("ConsoleVerbositySelect.Items4"),
            resources.GetString("ConsoleVerbositySelect.Items5")});
            resources.ApplyResources(this.ConsoleVerbositySelect, "ConsoleVerbositySelect");
            this.ConsoleVerbositySelect.Name = "ConsoleVerbositySelect";
            this.ConsoleVerbositySelect.SelectedIndexChanged += new System.EventHandler(this.ConsoleVerbositySelect_SelectedIndexChanged);
            // 
            // LabelConsoleVerbosity
            // 
            resources.ApplyResources(this.LabelConsoleVerbosity, "LabelConsoleVerbosity");
            this.LabelConsoleVerbosity.Name = "LabelConsoleVerbosity";
            // 
            // LogFileVerbositySelect
            // 
            this.LogFileVerbositySelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.LogFileVerbositySelect.FormattingEnabled = true;
            this.LogFileVerbositySelect.Items.AddRange(new object[] {
            resources.GetString("LogFileVerbositySelect.Items"),
            resources.GetString("LogFileVerbositySelect.Items1"),
            resources.GetString("LogFileVerbositySelect.Items2"),
            resources.GetString("LogFileVerbositySelect.Items3"),
            resources.GetString("LogFileVerbositySelect.Items4"),
            resources.GetString("LogFileVerbositySelect.Items5")});
            resources.ApplyResources(this.LogFileVerbositySelect, "LogFileVerbositySelect");
            this.LogFileVerbositySelect.Name = "LogFileVerbositySelect";
            this.LogFileVerbositySelect.SelectedIndexChanged += new System.EventHandler(this.LogFileVerbositySelect_SelectedIndexChanged);
            // 
            // LabelDebugFileVerb
            // 
            resources.ApplyResources(this.LabelDebugFileVerb, "LabelDebugFileVerb");
            this.LabelDebugFileVerb.Name = "LabelDebugFileVerb";
            // 
            // ButtonLaunch
            // 
            resources.ApplyResources(this.ButtonLaunch, "ButtonLaunch");
            this.ButtonLaunch.Name = "ButtonLaunch";
            this.ButtonLaunch.UseVisualStyleBackColor = true;
            this.ButtonLaunch.Click += new System.EventHandler(this.ButtonLaunch_Click);
            // 
            // NewVerLink
            // 
            resources.ApplyResources(this.NewVerLink, "NewVerLink");
            this.NewVerLink.Name = "NewVerLink";
            this.NewVerLink.TabStop = true;
            this.NewVerLink.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.NewVerLink_LinkClicked);
            // 
            // Setup
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.NewVerLink);
            this.Controls.Add(this.ButtonLaunch);
            this.Controls.Add(this.ConfigTabControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "Setup";
            this.Load += new System.EventHandler(this.Setup_Load);
            this.ConfigTabControl.ResumeLayout(false);
            this.TabGeneralSettings.ResumeLayout(false);
            this.TabGeneralSettings.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InputInstanceCount)).EndInit();
            this.TabController.ResumeLayout(false);
            this.ControllerOptionsPanel.ResumeLayout(false);
            this.ControllerOptionsPanel.PerformLayout();
            this.TabDebugging.ResumeLayout(false);
            this.TabDebugging.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl ConfigTabControl;
        private System.Windows.Forms.TabPage TabGeneralSettings;
        private System.Windows.Forms.NumericUpDown InputInstanceCount;
        private System.Windows.Forms.Label LabelInstances;
        private System.Windows.Forms.Button ButtonLaunch;
        private System.Windows.Forms.ComboBox InputResoSelect;
        private System.Windows.Forms.Label LabelResolution;
        private System.Windows.Forms.TabPage TabController;
        private System.Windows.Forms.Panel ControllerOptionsPanel;
        private System.Windows.Forms.Label LabelSelectController;
        private System.Windows.Forms.ComboBox InputControllerSelect;
        private System.Windows.Forms.Button ControllerBTResetAll;
        private System.Windows.Forms.ListView ControllerPlayerList;
        private System.Windows.Forms.CheckBox InputSkipIntros;
        private System.Windows.Forms.LinkLabel NewVerLink;
        private System.Windows.Forms.TabPage TabDebugging;
        private System.Windows.Forms.ComboBox LogFileVerbositySelect;
        private System.Windows.Forms.Label LabelDebugFileVerb;
        private System.Windows.Forms.ComboBox ConsoleVerbositySelect;
        private System.Windows.Forms.Label LabelConsoleVerbosity;
        private System.Windows.Forms.CheckBox InputBackgroundSound;
    }
}