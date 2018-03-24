using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace UpdatedMiniWord
{
    public partial class Form1 : Form
    {
        private String title = "Untitled";  //保存打开的文件的标题
        Encoding ec=Encoding.UTF8;          //设置文本的格式为 UTF-8
        string contents5;
        int len, now;
        public Form1()
        {
            InitializeComponent();
        }

        private void openFileToolStripButton_Click(object sender, EventArgs e)
        {
            /**
             * openFileDialog1 是在设计界面拖出来的控件 OpenFileDialog
             * 
             * 主要是打开 rtf 格式的文件
             */
            openFileDialog1.Filter = "文本文件|*.txt;*.html;*.docx;*.doc;*.rtf|所有文件|*.*"; //文件打开的过滤器
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                title = openFileDialog1.FileName;
                this.Text = title;                  //显示打开的文件名
                richTextBox1.Modified = false;
                string ext = title.Substring(title.LastIndexOf(".") + 1);//获取文件格式
                ext = ext.ToLower();
                FileStream fs = new FileStream(title, FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs, ec);
                if (ext == "rtf")  //如果后缀是 rtf 加载文件进来
                {
                    richTextBox1.LoadFile(title, RichTextBoxStreamType.RichText);
                }
                else
                {
                    richTextBox1.Text = sr.ReadToEnd();
                }
                fs.Close();
                sr.Close();
            }
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            if (title == "Untitled")//如果是不是打开文件然后编辑的话 点击保存就相当于另存为咯
                另存为AToolStripMenuItem_Click(sender, e);//这个另存为的按钮是在顶部的菜单栏
            else
            {
                string ext = title.Substring(title.LastIndexOf(".") + 1);
                ext = ext.ToLower();
                if (ext == "rtf")//按照不同的格式保存文件
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.RichText);
                else if (ext == "doc" || ext == "txt")
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                else if (ext == "uni")
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.UnicodePlainText);
                else
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                richTextBox1.Modified = false;
            }
        }

        private void 另存为AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /**
             * saveFileDialog1 同样也是设计窗体界面拖出来的控件
             */
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                title = saveFileDialog1.FileName;
                this.Text = title;
                switch (saveFileDialog1.FilterIndex)
                {
                    case 1:
                        richTextBox1.SaveFile(title, RichTextBoxStreamType.RichText);
                        break;
                    case 2:
                        richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                        break;
                    case 3:
                        richTextBox1.SaveFile(title, RichTextBoxStreamType.UnicodePlainText);
                        break;
                    default:
                        richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                        break;
                }
                richTextBox1.Modified = false;
            }
        }

        private void boldToolStripButton_Click(object sender, EventArgs e)
        {
            richTextBox1.SelectionFont = new Font(richTextBox1.SelectionFont, richTextBox1.SelectionFont.Style ^ FontStyle.Bold);
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            richTextBox1.SelectionFont = new Font(richTextBox1.SelectionFont, richTextBox1.SelectionFont.Style ^ FontStyle.Italic);
        }

        private void toolStripButton8_Click(object sender, EventArgs e)
        {
            richTextBox1.SelectionFont = new Font(richTextBox1.SelectionFont, richTextBox1.SelectionFont.Style ^ FontStyle.Underline);
        }

        private void cutToolStripButton_Click(object sender, EventArgs e)
        {
            richTextBox1.Cut();
        }

        private void toolStripButton10_Click(object sender, EventArgs e)
        {
            richTextBox1.Copy();
        }

        private void toolStripButton11_Click(object sender, EventArgs e)
        {
            richTextBox1.Paste();
        }

        private void toolStripButton12_Click(object sender, EventArgs e)
        {
            Bitmap bmp;
            if (openImageDialog.ShowDialog() == DialogResult.OK)
            {
                string filename = openImageDialog.FileName;
                try
                {
                    bmp = new Bitmap(filename);//文件转化为 Bitmap
                    Clipboard.SetDataObject(bmp);
                    DataFormats.Format dft = DataFormats.GetFormat(DataFormats.Bitmap);
                    if (this.richTextBox1.CanPaste(dft))
                        richTextBox1.Paste(dft);    //图片加入到富文本中去
                }
                catch (Exception ex)
                {
                    MessageBox.Show("图片插入失败" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private void toolStripButton15_Click(object sender, EventArgs e)
        {
            /**
             * colorDialog1 是设计界面拖出来的控件
             */
            if (colorDialog1.ShowDialog() == DialogResult.OK)
                richTextBox1.SelectionColor = colorDialog1.Color;//直接设置选中的字段的颜色
        }

        private void toolStripButton19_Click(object sender, EventArgs e)
        {
            richTextBox1.Undo();
        }

        private void toolStripButton20_Click(object sender, EventArgs e)
        {
            richTextBox1.Redo();
        }

        private void toolStripButton21_Click(object sender, EventArgs e)
        {
            this.richTextBox1.SelectionAlignment = HorizontalAlignment.Left;
        }

        private void toolStripButton22_Click(object sender, EventArgs e)
        {
            this.richTextBox1.SelectionAlignment = HorizontalAlignment.Center;
        }

        private void toolStripButton23_Click(object sender, EventArgs e)
        {
            this.richTextBox1.SelectionAlignment = HorizontalAlignment.Right;
        }

        private void toolStripButton25_Click(object sender, EventArgs e)
        {
            ZoomInOut(false);//增加
        }

        private void toolStripButton26_Click(object sender, EventArgs e)
        {
            ZoomInOut(true);//减小
        }
        private void ZoomInOut(bool IsZoomOut)
        {
            float zoom = 0;
            zoom = this.richTextBox1.ZoomFactor;
            if (IsZoomOut)
                zoom -= (float)0.1;
            else
                zoom += (float)0.1;
            if (zoom < 0.64 || zoom > 64)
                return;
            this.richTextBox1.ZoomFactor = zoom;
        }

        private void cbFamily_SelectedIndexChanged(object sender, EventArgs e)
        {//Font Family
            if (this.cbFamily.SelectedItem == null) return;
            string ss = this.cbFamily.SelectedItem.ToString().Trim();
            richTextBox1.SelectionFont = new Font(ss, richTextBox1.SelectionFont.Size, richTextBox1.SelectionFont.Style);
        }

        private void cbSize_SelectedIndexChanged(object sender, EventArgs e)
        {//设置字体大小
            if (cbSize.SelectedItem == null) return;
            int size = Convert.ToInt32(this.cbSize.SelectedItem.ToString().Trim());
            richTextBox1.SelectionFont = new Font(richTextBox1.SelectionFont.FontFamily, size, richTextBox1.SelectionFont.Style);
        }

        private void 插入图片IToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Bitmap bmp;
            if (openImageDialog.ShowDialog() == DialogResult.OK)
            {
                string filename = openImageDialog.FileName;
                try
                {
                    bmp = new Bitmap(filename);//文件转化为 Bitmap
                    Clipboard.SetDataObject(bmp);
                    DataFormats.Format dft = DataFormats.GetFormat(DataFormats.Bitmap);
                    if (this.richTextBox1.CanPaste(dft))
                        richTextBox1.Paste(dft);    //图片加入到富文本中去
                }
                catch (Exception ex)
                {
                    MessageBox.Show("图片插入失败" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private void 开始执行不调试ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 打开ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "文本文件|*.txt;*.html;*.docx;*.doc;*.rtf|所有文件|*.*"; //文件打开的过滤器
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                title = openFileDialog1.FileName;
                this.Text = title;                  //显示打开的文件名
                richTextBox1.Modified = false;
                string ext = title.Substring(title.LastIndexOf(".") + 1);//获取文件格式
                ext = ext.ToLower();
                FileStream fs = new FileStream(title, FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs, ec);
                if (ext == "rtf")  //如果后缀是 rtf 加载文件进来
                {
                    richTextBox1.LoadFile(title, RichTextBoxStreamType.RichText);
                }
                else
                {
                    richTextBox1.Text = sr.ReadToEnd();
                }
                fs.Close();
                sr.Close();
            }
        }

        private void 保存SToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (title == "Untitled")//如果是不是打开文件然后编辑的话 点击保存就相当于另存为咯
                另存为AToolStripMenuItem_Click(sender, e);//这个另存为的按钮是在顶部的菜单栏
            else
            {
                string ext = title.Substring(title.LastIndexOf(".") + 1);
                ext = ext.ToLower();
                if (ext == "rtf")//按照不同的格式保存文件
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.RichText);
                else if (ext == "doc" || ext == "txt")
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                else if (ext == "uni")
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.UnicodePlainText);
                else
                    richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                richTextBox1.Modified = false;
            }
        }

        private void 退出EToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 剪切ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Cut();
        }

        private void 复制PToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Copy();
        }

        private void 粘贴ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Paste();
        }

        private void 清楚ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 替换ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (now >= len)
            {
                MessageBox.Show("程序已经运行结束!");
            }
            else
            {
                string s = "";
                int id = 0;
                while (now < len && contents5[now] >= '0' && contents5[now] <= '9')
                {
                    id = id * 10 + contents5[now] - '0';
                    now++;
                }
                while (now < len && (contents5[now] < '0' || contents5[now] > '9')) now++;
                while (now < len)
                {
                    if (contents5[now] == '#')
                    {
                        now++;
                        while (contents5[now] < '0' || contents5[now] > '9') now++;
                        break;
                    }
                    s += contents5[now];
                    now++;
                }
                richTextBox6.Text = s;

                int length = richTextBox7.Text.Length;
                int l = 0, r = 0, cnt = 0;
                for (int i = 0; i < length; ++i)
                {
                    if (richTextBox7.Text[i] == '\n')
                    {
                        if (cnt == id)
                        {
                            r = i;
                            break;
                        }
                        l = i + 1;
                        cnt++;
                    }
                }
                richTextBox7.SelectionStart = 0;
                richTextBox7.SelectionLength = length;
                richTextBox7.SelectionColor = Color.Black;
                richTextBox7.SelectionStart = l;
                richTextBox7.SelectionLength = r - l;
                richTextBox7.SelectionColor = Color.Red;
            }
        }

        private void 调试ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (title == "Untitled")
            {
                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    title = saveFileDialog1.FileName;
                    this.Text = title;
                    switch (saveFileDialog1.FilterIndex)
                    {
                        case 1:
                            richTextBox1.SaveFile(title, RichTextBoxStreamType.RichText);
                            break;
                        case 2:
                            richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                            break;
                        case 3:
                            richTextBox1.SaveFile(title, RichTextBoxStreamType.UnicodePlainText);
                            break;
                        default:
                            richTextBox1.SaveFile(title, RichTextBoxStreamType.PlainText);
                            break;
                    }
                    richTextBox1.Modified = false;
                }
            }
            string fileName = @"sw_compiler.exe";
            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = fileName;
            p.StartInfo.CreateNoWindow = false;
            p.StartInfo.Arguments = title;
            p.Start();
            //Thread.Sleep(3000);
            p.WaitForExit();

            string ferror = @"ferror.txt";
            byte[] b = new byte[1024 * 1024];
            FileStream fileRead = new FileStream(ferror, FileMode.OpenOrCreate, FileAccess.Read);
            int r = fileRead.Read(b, 0, b.Length);
            fileRead.Close();
            fileRead.Dispose();
            string contents = Encoding.Default.GetString(b, 0, r);
            richTextBox2.Text = contents;

            if (r > 0) MessageBox.Show("编译失败", "编译结果");
            else MessageBox.Show("编译成功", "编译结果");

            string fresult = @"fresult.txt";
            byte[] c = new byte[1024 * 1024];
            FileStream fileRead1 = new FileStream(fresult, FileMode.OpenOrCreate, FileAccess.Read);
            int r1 = fileRead1.Read(c, 0, c.Length);
            fileRead1.Close();
            fileRead1.Dispose();
            string contents1 = Encoding.Default.GetString(c, 0, r1);
            richTextBox5.Text = contents1;

            string fcode = @"fcode.txt";
            byte[] c2 = new byte[1024 * 1024];
            FileStream fileRead2 = new FileStream(fcode, FileMode.OpenOrCreate, FileAccess.Read);
            int r2 = fileRead2.Read(c2, 0, c2.Length);
            fileRead2.Close();
            fileRead2.Dispose();
            string contents2 = Encoding.Default.GetString(c2, 0, r2);
            richTextBox7.Text = contents2;

            string ftable = @"ftable.txt";
            byte[] c3 = new byte[1024 * 1024];
            FileStream fileRead3 = new FileStream(ftable, FileMode.OpenOrCreate, FileAccess.Read);
            int r3 = fileRead3.Read(c3, 0, c3.Length);
            fileRead3.Close();
            fileRead3.Dispose();
            string contents3 = Encoding.Default.GetString(c3, 0, r3);
            richTextBox4.Text = contents3;

            string fdebug = @"fdebug.txt";
            byte[] b5 = new byte[1024 * 1024];
            FileStream fileRead5 = new FileStream(fdebug, FileMode.OpenOrCreate, FileAccess.Read);
            int r5 = fileRead5.Read(b5, 0, b5.Length);
            fileRead5.Close();
            fileRead5.Dispose();
            contents5 = Encoding.Default.GetString(b5, 0, r5);
            len = contents5.Length;
            now = 0;
            //p.Kill();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            now = 0;
            richTextBox6.Text = "";
            int length = richTextBox7.Text.Length;
            richTextBox7.SelectionStart = 0;
            richTextBox7.SelectionLength = length;
            richTextBox7.SelectionColor = Color.Black;
        }

        private void 编码ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
    }
}
