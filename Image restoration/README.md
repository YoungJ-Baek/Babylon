# Image restoration
In this folder, I introduce three filters.

#### Band reject filter
#### Mean filter
#### Median filter

In band reject filter, there are two functions which reject in circle and box shape in frequency domain. However, in this codes, there is no function about FFT or DFT. You can easily get codes from Internet about them in few minutes. So you can get and use them. I recommend using FFT rather than DFT.

## Band reject filter
### Band reject filter in circle shape

#### Left top  : original image
#### Right top: FFT image(centralized image)
#### Right bottom: band reject image(frequency domain)
#### Left bottom  : restoration image

![image01](https://user-images.githubusercontent.com/54461378/87754662-5f445f80-c840-11ea-972d-25eaf7a343e3.png)


### Band reject filter in box shape

#### Left top  : original image
#### Right top: FFT image(centralized image)
#### Right bottom: band reject image(frequency domain)
#### Left bottom  : restoration image

![image02](https://user-images.githubusercontent.com/54461378/87754724-8438d280-c840-11ea-866d-d35ea0ccaa38.png)


## Mean filter

#### Left   : original
#### Right : restoration image

![0](https://user-images.githubusercontent.com/54461378/87754916-eb568700-c840-11ea-9d04-594a388670d8.png)


## Median filter

#### Left   : original
#### Right : restoration image

![1](https://user-images.githubusercontent.com/54461378/87754940-f8737600-c840-11ea-9111-6f5d73478312.png)