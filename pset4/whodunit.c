/*
 * here's all i did in whodunit.c
 */

// change red pixels to black
  if (triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00 && triple.rgbtRed == 0xFF)
  {
      triple.rgbtRed = 0x00;
  }
            
// change white pixels to black
  if (triple.rgbtBlue == 0xFF && triple.rgbtGreen == 0xFF && triple.rgbtRed == 0xFF)
  {
      triple.rgbtBlue = 0x00;
      triple.rgbtGreen = 0x00;
      triple.rgbtRed = 0x00;
  }
