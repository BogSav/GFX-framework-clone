#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture


float getGrayValue(vec4 color)
{
    return 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
}

vec4 grayscale()
{
    float gray = getGrayValue(texture(textureImage, textureCoord));
    return vec4(gray, gray, gray,  0);
}

vec4 blur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
        }
    }
        
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

void bubbleSort(inout vec4 arr[9], int n)
{
    int i, j;
    bool swapped;

    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (getGrayValue(arr[j]) > getGrayValue(arr[j + 1])){
                vec4 aux = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = aux;
                swapped = true;
            }
        }

        if (swapped == false)
            break;
    }
}

vec4 median()
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    vec4 a[9];
    for(int i = 0; i <= 2; i++)
    {
        for(int j = 0; j <= 2; j++)
        {
            a[i * 3 + j] = texture(textureImage, textureCoord + vec2(i - 1, j - 1) * texelSize);
        }
    }

    bubbleSort(a, 9);

    return a[4];
}

float sobel()
{
    vec2 texelSize = 1.0f / screenSize;
    vec3 Gx[3];
    Gx[0] = vec3(-1, 0, 1);
    Gx[1] = vec3(-2, 0, 2);
    Gx[2] = vec3(-1, 0, 1);

    vec3 Gy[3];
    Gy[0] = vec3(1, 2, 1);
    Gy[1] = vec3(0, 0, 0);
    Gy[2] = vec3(-1, -2, -1);

    float sumX = 0.0;
    float sumY = 0.0;

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            float gray = getGrayValue(texture(textureImage, textureCoord + vec2(i, j) * texelSize));
            sumX += gray * Gx[i + 1][j + 1];
            sumY += gray * Gy[i + 1][j + 1];
        }
    }

    return sqrt(sumX * sumX + sumY * sumY);
}

vec4 threshold(float thresholdValue)
{
    float mag = sobel();

    if(mag > thresholdValue)
        return vec4(1.0, 1.0, 1.0, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}


void main()
{
    switch (outputMode)
    {
        case 1:
        {
            out_color = grayscale();
            break;
        }

        case 2:
        {
            out_color = blur(3);
            break;
        }

        case 3:
        {
            out_color = median();
            break;
        }

        case 4:
        {
            out_color = threshold(0.1);
            break;
        }

        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
}
