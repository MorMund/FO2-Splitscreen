namespace Launcher
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
            this.ButtonLaunch = new System.Windows.Forms.Button();
            this.InputAttachConsole = new System.Windows.Forms.CheckBox();
            this.ConfigTabControl.SuspendLayout();
            this.TabGeneralSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InputInstanceCount)).BeginInit();
            this.TabController.SuspendLayout();
            this.ControllerOptionsPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // ConfigTabControl
            // 
            this.ConfigTabControl.Controls.Add(this.TabGeneralSettings);
            this.ConfigTabControl.Controls.Add(this.TabController);
            resources.ApplyResources(this.ConfigTabControl, "ConfigTabControl");
            this.ConfigTabControl.Name = "ConfigTabControl";
            this.ConfigTabControl.SelectedIndex = 0;
            this.ConfigTabControl.Enter += new System.EventHandler(this.ConfigTabControl_Enter);
            // 
            // TabGeneralSettings
            // 
            this.TabGeneralSettings.Controls.Add(this.InputAttachConsole);
            this.TabGeneralSettings.Controls.Add(this.InputResoSelect);
            this.TabGeneralSettings.Controls.Add(this.InputInstanceCount);
            this.TabGeneralSettings.Controls.Add(this.LabelResolution);
            this.TabGeneralSettings.Controls.Add(this.LabelInstances);
            resources.ApplyResources(this.TabGeneralSettings, "TabGeneralSettings");
            this.TabGeneralSettings.Name = "TabGeneralSettings";
            this.TabGeneralSettings.UseVisualStyleBackColor = true;
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
            this.ControllerPlayerList.MultiSelect = false;
            this.ControllerPlayerList.Name = "ControllerPlayerList";
            this.ControllerPlayerList.UseCompatibleStateImageBehavior = false;
            this.ControllerPlayerList.SelectedIndexChanged += new System.EventHandler(this.ControllerPlayerList_SelectedIndexChanged);
            // 
            // ButtonLaunch
            // 
            resources.ApplyResources(this.ButtonLaunch, "ButtonLaunch");
            this.ButtonLaunch.Name = "ButtonLaunch";
            this.ButtonLaunch.UseVisualStyleBackColor = true;
            this.ButtonLaunch.Click += new System.EventHandler(this.ButtonLaunch_Click);
            // 
            // InputAttachConsole
            // 
            resources.ApplyResources(this.InputAttachConsole, "InputAttachConsole");
            this.InputAttachConsole.Name = "InputAttachConsole";
            this.InputAttachConsole.UseVisualStyleBackColor = true;
            this.InputAttachConsole.CheckedChanged += new System.EventHandler(this.InputAttachConsole_CheckedChanged);
            // 
            // Setup
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
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
            this.ResumeLayout(false);

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
        private System.Windows.Forms.CheckBox InputAttachConsole;
    }
}