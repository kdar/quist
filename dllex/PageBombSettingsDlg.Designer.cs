namespace Quist
{
  partial class PageBombSettingsDlg
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
      if (disposing && (components != null)) {
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PageBombSettingsDlg));
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.numericUpDownInterval = new System.Windows.Forms.NumericUpDown();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.numericUpDownPages = new System.Windows.Forms.NumericUpDown();
      this.buttonSave = new System.Windows.Forms.Button();
      this.buttonCancel = new System.Windows.Forms.Button();
      this.buttonSaveAndSend = new System.Windows.Forms.Button();
      this.groupBox1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterval)).BeginInit();
      this.groupBox2.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPages)).BeginInit();
      this.SuspendLayout();
      // 
      // groupBox1
      // 
      this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox1.Controls.Add(this.numericUpDownInterval);
      this.groupBox1.Location = new System.Drawing.Point(12, 70);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(294, 53);
      this.groupBox1.TabIndex = 0;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Interval (in milliseconds)";
      // 
      // numericUpDownInterval
      // 
      this.numericUpDownInterval.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.numericUpDownInterval.Location = new System.Drawing.Point(6, 19);
      this.numericUpDownInterval.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
      this.numericUpDownInterval.Name = "numericUpDownInterval";
      this.numericUpDownInterval.Size = new System.Drawing.Size(282, 20);
      this.numericUpDownInterval.TabIndex = 5;
      // 
      // groupBox2
      // 
      this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox2.Controls.Add(this.numericUpDownPages);
      this.groupBox2.Location = new System.Drawing.Point(12, 13);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(294, 51);
      this.groupBox2.TabIndex = 1;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Number of pages to send";
      // 
      // numericUpDownPages
      // 
      this.numericUpDownPages.Location = new System.Drawing.Point(6, 19);
      this.numericUpDownPages.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
      this.numericUpDownPages.Name = "numericUpDownPages";
      this.numericUpDownPages.Size = new System.Drawing.Size(282, 20);
      this.numericUpDownPages.TabIndex = 0;
      this.numericUpDownPages.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
      // 
      // buttonSave
      // 
      this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.buttonSave.Location = new System.Drawing.Point(138, 141);
      this.buttonSave.Name = "buttonSave";
      this.buttonSave.Size = new System.Drawing.Size(75, 23);
      this.buttonSave.TabIndex = 2;
      this.buttonSave.Text = "Save";
      this.buttonSave.UseVisualStyleBackColor = true;
      this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
      // 
      // buttonCancel
      // 
      this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.buttonCancel.Location = new System.Drawing.Point(225, 141);
      this.buttonCancel.Name = "buttonCancel";
      this.buttonCancel.Size = new System.Drawing.Size(75, 23);
      this.buttonCancel.TabIndex = 3;
      this.buttonCancel.Text = "Cancel";
      this.buttonCancel.UseVisualStyleBackColor = true;
      this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
      // 
      // buttonSaveAndSend
      // 
      this.buttonSaveAndSend.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.buttonSaveAndSend.Location = new System.Drawing.Point(12, 141);
      this.buttonSaveAndSend.Name = "buttonSaveAndSend";
      this.buttonSaveAndSend.Size = new System.Drawing.Size(111, 23);
      this.buttonSaveAndSend.TabIndex = 4;
      this.buttonSaveAndSend.Text = "Save and Send";
      this.buttonSaveAndSend.UseVisualStyleBackColor = true;
      this.buttonSaveAndSend.Click += new System.EventHandler(this.buttonSaveAndSend_Click);
      // 
      // PageBombSettingsDlg
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(318, 180);
      this.Controls.Add(this.buttonSaveAndSend);
      this.Controls.Add(this.buttonCancel);
      this.Controls.Add(this.buttonSave);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MaximizeBox = false;
      this.Name = "PageBombSettingsDlg";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Page Bomb settings";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PageBombSettingsDlg_FormClosing);
      this.groupBox1.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterval)).EndInit();
      this.groupBox2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPages)).EndInit();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.NumericUpDown numericUpDownInterval;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.Button buttonSave;
    private System.Windows.Forms.Button buttonCancel;
    private System.Windows.Forms.Button buttonSaveAndSend;
    private System.Windows.Forms.NumericUpDown numericUpDownPages;
  }
}