#include "MorphologyProc.h"
#include "RWImage.h"



MorphologyProc::MorphologyProc(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image = rwi.readRawImage(path, b);
}

bool MorphologyProc::isConditionalPatternMatch(int x, int y, int type){
    int patterns_thin[46] = {
        0b010011000, 0b010110000, 0b000110010, 0b000011010,
        0b001011001, 0b111010000, 0b100110100, 0b000010111,
        0b110011000, 0b010011001, 0b011110000, 0b001011010,
        0b011011000, 0b110110000, 0b000110110, 0b000011011,
        0b110011001, 0b011110100, 0b111011000, 0b011011001,
        0b111110000, 0b110110100, 0b100110110, 0b000110111,
        0b000011111, 0b001011011,

        0b111011001, 0b111110100, 0b100110111, 0b001011111,
        0b011011011, 0b111111000, 0b110110110, 0b000111111,
        0b111011011, 0b011011111, 0b111111100, 0b111111001,
        0b111110110, 0b110110111, 0b100111111, 0b001111111,
        0b111011111, 0b111111101, 0b111110111, 0b101111111
    };

    int patterns_shrink[58] = {
        0b001010000, 0b100010000, 0b000010100, 0b000010001,
        0b000011000, 0b010010000, 0b000110000, 0b000010010,
        0b001011000, 0b011010000, 0b110010000, 0b100110000,
        0b000110100, 0b000010110, 0b000010011, 0b000011001,
        0b110011000, 0b010011001, 0b011110000, 0b001011010,
        0b011011000, 0b110110000, 0b000110110, 0b000011011,
        0b110011001, 0b011110100,

        0b011011011, 0b111111000, 0b110110110, 0b000111111,
        0b111011011, 0b011011111, 0b111111100, 0b111111001,
        0b111110110, 0b110110111, 0b100111111, 0b001111111,
        0b111011111, 0b111111101, 0b111110111, 0b101111111,
        0b001011001, 0b111010000, 0b100110100, 0b000010111,
        0b111011000, 0b011011001, 0b111110000, 0b110110100,
        0b100110110, 0b000110111, 0b000011111, 0b001011011,
        0b111011001, 0b111110100, 0b100110111, 0b001011111
    };

    int pattern = 0b000000000;
    // loop through 3x3 area and fill the pattern
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            pattern = pattern << 1;
            if (image[x + i][y + j][0] == 255){
                pattern += 1;
            }
        }
    }

    if(type == 0){
        for(int i = 0; i < 46; i++){
            if(patterns_thin[i] == pattern){
                return true;
            }
        }
    }else{
        for(int i = 0; i < 58; i++){
            if(patterns_shrink[i] == pattern){
                return true;
            }
        }
    }

    
        
    return false;
}

bool MorphologyProc::isUnconditionalPatternMatch(int x, int y, std::vector<std::vector<unsigned char> >  &M){
    int ST_no_D[28] = {
        0b001010000, 0b100010000, 0b000010010, 0b000011000,                               
        0b001011000, 0b011010000, 0b110010000, 0b100110000, 
        0b000110100, 0b000010110, 0b000010011, 0b000011001,
        0b011110000, 0b110011000, 0b010011001, 0b001011010,
        0b011011100, 0b001011100, 0b011010100, 0b110110001, 
        0b100110001, 0b110010001, 0b001110110, 0b001110100, 
        0b001010110, 0b100011011, 0b100011001, 0b100010011
    };
    int ST_with_D[42] = {
        0b110110000, 0b000011011,
        0b010111000, 0b010111000, 0b000111010, 0b000111010,
        0b010110010, 0b010110010, 0b010011010, 0b010011010,
        0b101010001, 0b101010010, 0b101010011, 0b101010100,
        0b101010101, 0b101010110, 0b101010111, 0b100010101, 
        0b100011100, 0b100011101, 0b101010100, 0b101010101, 
        0b101011100, 0b101011101, 0b001010101, 0b010010101, 
        0b011010101, 0b100010101, 0b101010101, 0b110010101,
        0b111010101, 0b001010101, 0b001110001, 0b001110101, 
        0b101010001, 0b101010101, 0b101110001, 0b101110101,
        0b010011100, 0b010110001, 0b001110010, 0b100011010      
    };
    int ST_mask[42] = {
        0b110110000, 0b000011011,                           
        0b011111011, 0b110111110, 0b110111110, 0b011111011,
        0b010111111, 0b111111010, 0b111111010, 0b010111111, 
        0b101010111, 0b101010111, 0b101010111, 0b101010111,
        0b101010111, 0b101010111, 0b101010111, 0b101011111, 
        0b101011111, 0b101011111, 0b101011111, 0b101011111, 
        0b101011111, 0b101011111, 0b111010101, 0b111010101, 
        0b111010101, 0b111010101, 0b111010101, 0b111010101, 
        0b111010101, 0b101110101, 0b101110101, 0b101110101, 
        0b101110101, 0b101110101, 0b101110101, 0b101110101,
        0b011111110, 0b110111011, 0b011111110, 0b110111011 
    };

    int pattern = 0b000000000;
    // loop through 3x3 area and fill the pattern
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            pattern = pattern << 1;
            if (M[x + i][y + j] == 1){
                pattern += 1;
            }
        }
    }

    for (int i = 0; i < 28; i++){
        if (pattern == ST_no_D[i]){
            return true;
        }
    }
    for (int i = 0; i < 42; i++){
        int check = pattern & ST_mask[i];
        if (check == ST_with_D[i]){
            return true;
        }
    }
    

    return false;
}



void MorphologyProc::basic_morphological_process(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));
    RWImage rwi(height, width);
    binarize();
    zero_padding();
    int count = 1;
    int iteration = 20;
    while(count != 0){
        count = 0;
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,0)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                        count++;
                    }
                }        
            }
        }
        iteration--;
        if(iteration == 0){
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    result[i][j][0] = image[i + 1][j + 1][0];
                }
            }
            rwi.writeRawImage(outputPath + "intermediate.raw", result, 1);
        }
    }

    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[i][j][0] = image[i + 1][j + 1][0];
        }
    }

    
    rwi.writeRawImage(outputPath + "final.raw", result, 1);
}

void MorphologyProc::zero_padding(){
    std::vector<std::vector<std::vector<unsigned char> > > result(height + 2, std::vector<std::vector<unsigned char> >(width + 2, std::vector<unsigned char>(1, 0)));

    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            result[i][j][0] = image[i-1][j-1][0];
        }
    }

    image = result;
}

void MorphologyProc::de_zero_padding(){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[i][j][0] = image[i+1][j+1][0];
        }
    }

    image = result;
}

void MorphologyProc::binarize(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j][0] < 127){
                image[i][j][0] = 0;
            }else{
                image[i][j][0] = 255;
            }
        }
    }
}

void MorphologyProc::shape_detection1(std::string outputPath){
    RWImage rwi(height, width);

    binarize();

    zero_padding();

    // inverse black and white
    for(int i = 0; i <= height + 1 ; i++){
        for(int j = 0; j <= width + 1; j++){
            if(image[i][j][0] == 255){
                image[i][j][0] = 0;
            }else{
                image[i][j][0] = 255;
            }
        }
    }

    // shrinking
    int count1 = 1;
    while(count1 != 0){
        count1 = 0;
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,1)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                        count1++;
                    }
                }        
            }
        }
    }
    int countDot = 0;
    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(ifDot(i,j)){
                countDot++;
            }        
        }
    }

    std::cout << countDot << std::endl;

    de_zero_padding();
    rwi.writeRawImage(outputPath, image, 1);

}

bool MorphologyProc::ifDot(int x, int y){
    if(image[x - 1][y - 1][0] == 0 && image[x - 1][y][0] == 0 && image[x - 1][y + 1][0] == 0 &&
    image[x][y - 1][0] == 0 && image[x][y + 1][0] == 0 && image[x + 1][y - 1][0] == 0 && 
    image[x + 1][y][0] == 0 && image[x + 1][y + 1][0] == 0 && image[x][y][0] == 255){
        return true;
    }

    return false;
}

void MorphologyProc::shape_detection2(std::string outputPath){
    zero_padding();

    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(ifDot(i,j)){
                image[i][j][0] = 0;
            }        
        }
    }

    de_zero_padding();

    // inverse black and white
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j][0] == 255){
                image[i][j][0] = 0;
            }else{
                image[i][j][0] = 255;
            }
        }
    }

    zero_padding();

    std::vector<std::vector<std::vector<unsigned char> > > temp(height + 2, std::vector<std::vector<unsigned char> >(width + 2, std::vector<unsigned char>(1, 0)));
    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(image[i][j][0] == 0){
                temp[i - 1][j - 1][0] = 0;
                temp[i - 1][j][0] = 0;
                temp[i - 1][j + 1][0] = 0;
                temp[i][j - 1][0] = 0;
                temp[i][j][0] = 0;
                temp[i][j + 1][0] = 0;
                temp[i + 1][j - 1][0] = 0;
                temp[i + 1][j][0] = 0;
                temp[i + 1][j + 1][0] = 0;
            }else{
                temp[i][j][0] = 255;
            }
        }
    }
    image = temp;

    // shrinking
    int count2 = 1;
    while(count2 != 0){
        count2 = 0;
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,1)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                        count2++;
                    }
                }        
            }
        }
    }

    int countDot = 0;
    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(ifDot(i,j)){
                countDot++;
            }        
        }
    }

    std::cout << countDot << std::endl;

    de_zero_padding();
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, image, 1);

}


void MorphologyProc::fillHole(int x, int y, std::vector<std::vector<std::vector<unsigned char> > > &inputImage){
    inputImage[x][y][0] = 255;
    if((x - 1) >= 0 && (y - 1) >= 0 && inputImage[x-1][y-1][0] != 255){
        fillHole(x-1, y-1, inputImage);
    }
    if((x - 1) >= 0&& inputImage[x-1][y][0] != 255){
        fillHole(x - 1, y, inputImage);
    }
    if((x - 1) >= 0 && (y + 1) < width&& inputImage[x-1][y+1][0] != 255){
        fillHole(x - 1, y + 1, inputImage);
    }
    if((y - 1) >= 0 && inputImage[x][y-1][0] != 255){
        fillHole(x, y - 1, inputImage);
    }
    if((y + 1) < width && inputImage[x][y+1][0] != 255){
        fillHole(x, y + 1, inputImage);
    }
    if((x + 1) < height && (y - 1) >= 0 && inputImage[x+1][y-1][0] != 255){
        fillHole(x + 1, y - 1, inputImage);
    }
    if((x + 1) < height && inputImage[x+1][y][0] != 255){
        fillHole(x + 1, y, inputImage);
    }
    if((x + 1) < height && (y + 1) < width && inputImage[x+1][y+1][0] != 255){
        fillHole(x + 1, y + 1, inputImage);
    }
}

void MorphologyProc::shape_detection3(std::string outputPath){
    RWImage rwi(height, width);

    binarize();
    int circle = 0, rectangle = 0;

    zero_padding();

    std::vector<std::vector<std::vector<unsigned char> > > oriImage = image;

    // inverse black and white
    for(int i = 0; i <= height + 1 ; i++){
        for(int j = 0; j <= width + 1; j++){
            if(image[i][j][0] == 255){
                image[i][j][0] = 0;
            }else{
                image[i][j][0] = 255;
            }
        }
    }

    // shrinking
    int count1 = 1;
    while(count1 != 0){
        count1 = 0;
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,1)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                        count1++;
                    }
                }        
            }
        }
    }

    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(ifDot(i,j)){
                fillHole(i,j,oriImage);
            }        
        }
    }

    image = oriImage;

    // thinning
    int count2 = 1;
    while(count2 != 0){
        count2 = 0;
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,0)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                        count2++;
                    }
                }        
            }
        }
    }

    // shrinking
    int it = 2;
    while(it != 0){
        std::vector<std::vector<unsigned char> >  M(height + 2, std::vector<unsigned char>(width + 2, 0));
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(isConditionalPatternMatch(i,j,1)){
                    M[i][j] = 1;
                }
            }
        }
        for(int i = 1; i <= height; i++){
            for(int j = 1; j <= width; j++){
                if(M[i][j] == 1){
                    if(!isUnconditionalPatternMatch(i,j,M)){
                        image[i][j][0] = 0;
                    }
                }        
            }
        }
        it--;
    }

    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if(ifDot(i,j)){
                image[i][j][0] = 0;
                circle++;
            }        
        }
    }

    de_zero_padding();
    rwi.writeRawImage(outputPath, image, 1);

    rectangle = AllConnectedArea().size();

    std::cout << "circle:" << circle << std::endl;
    std::cout << "rectangle:" << rectangle << std::endl;
}

void MorphologyProc::DFS_helper(int x, int y, std::vector<std::vector<bool> >  &record, std::vector<int> &counts){
    counts.back()++;
    record[x][y] = true;
    if((x - 1) >= 0 && (y - 1) >= 0 && !record[x-1][y-1] && image[x-1][y-1][0] == 255){
        DFS_helper(x - 1, y - 1, record, counts);
    }
    if((x - 1) >= 0 && !record[x-1][y] && image[x-1][y][0] == 255){
        DFS_helper(x - 1, y , record, counts);
    }
    if((x - 1) >= 0 && (y + 1) < width && !record[x-1][y+1] && image[x-1][y+1][0] == 255){
        DFS_helper(x - 1, y + 1, record, counts);
    }
    if((y - 1) >= 0 && !record[x][y-1] && image[x][y-1][0] == 255){
        DFS_helper(x, y - 1, record, counts);
    }
    if((y + 1) < width && !record[x][y+1] && image[x][y+1][0] == 255){
        DFS_helper(x, y + 1, record, counts);
    }
    if((x + 1) < height && (y - 1) >= 0 && !record[x+1][y-1] && image[x+1][y-1][0] == 255){
        DFS_helper(x + 1, y - 1, record, counts);
    }
    if((x + 1) < height && !record[x+1][y] && image[x+1][y][0] == 255){
        DFS_helper(x + 1, y, record, counts);
    }
    if((x + 1) < height && (y + 1) < width && !record[x+1][y+1] && image[x+1][y+1][0] == 255){
        DFS_helper(x + 1, y + 1, record, counts);
    }
}

std::vector<int> MorphologyProc::AllConnectedArea(){
    std::vector<std::vector<bool> >  record(height, std::vector<bool>(width, false));
    std::vector<int> counts;
    for(int j = 0; j < width; j++){
        for(int i = 0; i < height; i++){
            if(!record[i][j]){
                if(image[i][j][0] == 255){
                    counts.push_back(0);
                    DFS_helper(i,j,record,counts);
                }else{
                    record[i][j] = true;
                }
            }
        }
    }
    return counts;
}

void MorphologyProc::object_segmentation(std::string outputPath, float threshold){
    RWImage rwi(height, width);

    int thresholdPixel = threshold * 255;

    std::vector<std::vector<std::vector<unsigned char> > > grayImage(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));
    // image transform 
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j][0] > thresholdPixel && image[i][j][1] > thresholdPixel && image[i][j][2] > thresholdPixel){
                grayImage[i][j][0] = 0;
            }else{
                grayImage[i][j][0] = 255;
            }
        }
    }

    image = grayImage;

    rwi.writeRawImage(outputPath, image, 1);

    std::vector<int> beanSize = AllConnectedArea();

    std::cout << "count: " << beanSize.size() << std::endl;

    std::map<int, int> keyValueMap;
    for(int i = 0; i < beanSize.size(); i++){
        keyValueMap[beanSize[i]] = (i + 1); 
    }

    for (const auto& pair : keyValueMap) {
        std::cout << "type" << pair.second << ": " << pair.first << std::endl;
    }

}