﻿using System;
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
        public static extern bool Process(string filename_tr, string filename_ex);
        
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

            dlg.DefaultExt = ".jpg";
            dlg.Filter = "Image documents (.jpg)|*.jpg";

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

        private void MenuItem_Click_4(object sender, RoutedEventArgs e)
        {
            List<string> founded_fonts_folders = new List<string>();

            using (var fbd = new FolderBrowserDialog())
            {
                DialogResult result = fbd.ShowDialog();

                if (result == System.Windows.Forms.DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath) && (ImageFileName != null))
                {
                    string[] dirs = Directory.GetDirectories(fbd.SelectedPath);

                    foreach (var dir in dirs)
                    {
                        string[] files = Directory.GetFiles(dir);
                    
                        foreach (var filename in files)
                        {
                            if (Process(ImageFileName, filename))
                                founded_fonts_folders.Add(System.IO.Path.GetDirectoryName(filename));
                        }
                    }
                }
            }

            if (founded_fonts_folders.Count > 0)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append("Some elements from the next font folders were found:");

                foreach (var font_folder in founded_fonts_folders)
                {
                    sb.Append("=> " + font_folder + "\n");
                }
            }
            else
                Output.Text = "No searched items found";
        }



    }
}
