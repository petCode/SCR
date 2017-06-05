using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;

namespace SC_Recognizer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        #region DLL public methods

        [DllImport("SCR_OpenCV.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Demo(string filename);

        [DllImport("SCR_OpenCV.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Process(string filename);
        
        #endregion

        private string ImageFileName; 

        double scale = 1.0;
        double minScale = 0.5;
        double maxScale = 5.0;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.DefaultExt = ".bmp";
            dlg.Filter = "Image documents (.bmp)|*.bmp";

            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                ImageFileName = dlg.FileName;

                BitmapImage bi3 = new BitmapImage();
                bi3.BeginInit();
                bi3.UriSource = new Uri(ImageFileName, UriKind.Relative);
                bi3.CacheOption = BitmapCacheOption.OnLoad;
                bi3.EndInit();
                SourceImage.Source = bi3;
            }
        }

        private void MenuItem_Click_1(object sender, RoutedEventArgs e)
        {
            SourceImage.Source = null;
        }

        private void MenuItem_Click_2(object sender, RoutedEventArgs e)
        {
            MainWindow1.Close();
        }
        
        private void SourceImage_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            var position = e.MouseDevice.GetPosition(SourceImage);

            if (e.Delta > 0)
                scale += 0.1;
            else
                scale -= 0.1;

            if (scale > maxScale)
                scale = maxScale;
            if (scale < minScale)
                scale = minScale;

            SourceImage.RenderTransform = new ScaleTransform(scale, scale);
        }

        private void MenuItem_Click_3(object sender, RoutedEventArgs e)
        {
            var families = Fonts.GetFontFamilies(@"C:\WINDOWS\Fonts\Arial.TTF");
            foreach (System.Windows.Media.FontFamily family in families)
            {
                var typefaces = family.GetTypefaces();
                foreach (Typeface typeface in typefaces)
                {
                    GlyphTypeface glyph;
                    typeface.TryGetGlyphTypeface(out glyph);
                    IDictionary<int, ushort> characterMap = glyph.CharacterToGlyphMap;

                    foreach (KeyValuePair<int, ushort> kvp in characterMap)
                    {
                        Console.WriteLine(String.Format("{0}:{1}", kvp.Key, kvp.Value));
                    }

                }
            }
        }

        private void MenuItem_Click_4(object sender, RoutedEventArgs e)
        {
            if (ImageFileName != null)
                Process(ImageFileName);
        }

        private void MenuItem_Click_5(object sender, RoutedEventArgs e)
        {
            //if (ImageFileName != null)
              //  ThresholdConversion(ImageFileName);

            Bitmap bmp = new Bitmap(ImageFileName);

            RectangleF rectf = new RectangleF(0, 0, 100, 100);

            Graphics g = Graphics.FromImage(bmp);

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            System.Drawing.FontFamily fontFamily = new System.Drawing.FontFamily(@"C:\Windows\Fonts\symbol.ttf");
            g.DrawString("abcdefghijklmnopqrstuvwxyz1234567890", new Font(fontFamily, 20), System.Drawing.Brushes.Black, rectf);
            
            g.Flush();

            using (MemoryStream memory = new MemoryStream())
            {
                bmp.Save(memory, System.Drawing.Imaging.ImageFormat.Png);
                memory.Position = 0;
                BitmapImage bitmapImage = new BitmapImage();
                bitmapImage.BeginInit();
                bitmapImage.StreamSource = memory;
                bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                bitmapImage.EndInit();

                SourceImage.Source = bitmapImage;
            }

            //SourceImage.Image = bmp;
        }

        
    }
}
