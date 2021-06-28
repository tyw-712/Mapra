#include "GreyScale.h"
GreyScale::formatType GreyScale::format = P2;

void GreyScale::setFormat(int num) 
{
    switch(num) {
        case 0: format = P2; break;
        case 1: format = P5; break;
        case 2: format = MHa; break;
        case 3: format = MHb; break;
        default: greyScaleError("ungueltige Format");
    }
}

int GreyScale::readInt(std::istream& s)
{
    int i;
    s >> std::ws;
    while (s.peek() == '#') {
        s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        s >> std::ws;
    }
    s >> i;

    return i;
}

// 7 -> 0
int GreyScale::readBit(std::istream& s, char& byte, int& bitPos) {
    if(bitPos == 7) {
        byte = s.get();
    }

    int bitVal = byte & (1 << bitPos);
    bitPos --;
    bitPos = bitPos == -1 ? 7 : bitPos;
    
    return bitVal;
}
int GreyScale::decode(std::istream &s,  char& byte, int& bitPos, Node& root) {
    if (root.p0 == NULL && root.p1 == NULL) {
        return root.value;
    }
    int i = readBit(s, byte, bitPos);
    if (i == 0)
        return decode(s, byte, bitPos, *(root.p0));
    else
        return decode(s, byte, bitPos, *(root.p1));
}
// 7 -> 0
int GreyScale::writeBit(std::ostream& o, char& byte, int bitVal, int& bitPos) {
    byte = byte | (bitVal<< bitPos);
    if(bitPos == 0) {
        o << byte;
        byte = 0b00000000; //reset
    }
    bitPos --;
    bitPos = bitPos == -1 ? 7 : bitPos;
    return 0;
}
int GreyScale::writeBitFlush(std::ostream& o, char& byte, int& bitPos) {
    //zero padding
    while(bitPos != 7) {
        writeBit(o, byte, 0, bitPos);
    }
    return 0;
}

GreyScale GreyScale::compTrans() const
{
    auto w = this->getWidth();
    auto h = this->getHeight();

    GreyScale im(h, w);
    for(int imh = 0; imh < im.getHeight(); imh++) {
        for(int imw = 0; imw < im.getWidth(); imw++) {
            im(imw,imh) = std::rint((*this)(imw, imh) * 255);
        }
    }
    GreyScale img(h, w);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++){
            if(y==0) {
                if (x == 0)
                    img(x, y) = (uint8_t) im(x, y);
                else
                    img(x, y) = (uint8_t) ((int) im(x, y) - (int) im(x-1, y));
            }
            else {
                if(x == 0)
                    img(x, y) = (uint8_t) ((int) im(x, y) - ((int) im(x, y-1) + (int) im(x+1, y-1))/2);
                else if(x == w-1)
                    img(x, y) = (uint8_t) ((int) im(x, y) - ((int) im(x-1, y) + (int) im(x-1, y-1) + (int) im(x, y-1))/3);
                else
                    img(x, y) = (uint8_t) ((int) im(x, y) - ((int) im(x-1, y) + (int) im(x-1, y-1) + (int) im(x, y-1) + (int) im(x+1, y-1))/4);					
            }
        }
    for(int imh = 0; imh < im.getHeight(); imh++) {
        for(int imw = 0; imw < im.getWidth(); imw++) {
            im(imw,imh) = img(imw, imh) / 255;
        }
    }
    return im;
}

GreyScale GreyScale::decompTrans() const
{
    auto w = this->getWidth();
    auto h = this->getHeight();

    GreyScale im(h, w);
    for(int imh = 0; imh < im.getHeight(); imh++) {
        for(int imw = 0; imw < im.getWidth(); imw++) {
            im(imw,imh) = std::rint((*this)(imw, imh) * 255);
        }
    }
    GreyScale img(h, w);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++){
            if(y==0) {
                if (x == 0)
                    img(x, y) = (uint8_t) im(x, y);
                else
                    img(x, y) = (uint8_t) ((int) im(x, y) + (int) img(x-1, y));
            }
            else {
                if(x == 0)
                    img(x, y) = (uint8_t) ((int) im(x, y) + ((int) img(x, y-1) + (int) img(x+1, y-1))/2);
                else if(x == w-1)
                    img(x, y) = (uint8_t) ((int) im(x, y) + ((int) img(x-1, y) + (int) img(x-1, y-1) + (int) img(x, y-1))/3);
                else
                    img(x, y) = (uint8_t) ((int) im(x, y) + ((int) img(x-1, y) + (int) img(x-1, y-1) + (int) img(x, y-1) + (int) img(x+1, y-1))/4);					
            }
        }

    for(int imh = 0; imh < im.getHeight(); imh++) {
        for(int imw = 0; imw < im.getWidth(); imw++) {
            im(imw,imh) = img(imw, imh) / 255;
        }
    }
    return im;
}


std::istream & GreyScale::readP2(std::istream& s, GreyScale& g) 
{
    int width, height, colourDepth;
    width = readInt(s);
    height = readInt(s);
    colourDepth = readInt(s);

    g.resize(height, width);

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if(s.good()) {
                g(w, h) = ((float) GreyScale::readInt(s)) / colourDepth;
            }
        }
    }
    return s;
}
std::istream & GreyScale::readP5(std::istream& s, GreyScale& g) 
{
    int width, height, colourDepth;
    width = readInt(s);
    height = readInt(s);
    colourDepth = readInt(s);

    g.resize(height, width);

    s >> std::ws;
    while (s.peek() == '#') {
        s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        s >> std::ws;
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
                char b;
                b = s.get();
                g(w, h) = (float) b / colourDepth;
        }
    }
    s >> std::ws;
    if(!s.eof())
        exit(1);
    return s;
}
std::istream & GreyScale::readMHa(std::istream& s, GreyScale& g) 
{
    uint16_t width, height;
    s.read((char*) &width, 2);
    s.read((char*) &height, 2);
    g.resize(ntohs(height), ntohs(width));

    std::vector<Node> histogramm(256);

    for (int i = 0; i < 256; i++) {
        histogramm[i].value = i;
        uint32_t freq;
        s.read((char*) &freq, 4);
        histogramm[i].freq = ntohl(freq);
    }

    Node root = buildTree(histogramm);
    char byte = 0b00000000;
    int bitPos = 7;
    for (int h = 0; h < g.getHeight(); h++) {
        for (int w = 0; w < g.getWidth(); w++) {
            float value = decode(s, byte, bitPos, root);
            g(w, h) = value / 255;
        }
    }

    return s;
}
std::istream & GreyScale::readMHb(std::istream& s, GreyScale& g) 
{
    uint16_t width, height;
    s.read((char*) &width, 2);
    s.read((char*) &height, 2);
    g.resize(ntohs(height), ntohs(width));

    std::vector<Node> histogramm(256);

    for (int i = 0; i < 256; i++) {
        histogramm[i].value = i;
        uint32_t freq;
        s.read((char*) &freq, 4);
        histogramm[i].freq = ntohl(freq);
    }

    Node root = buildTree(histogramm);
    char byte = 0b00000000;
    int bitPos = 7;
    for (int h = 0; h < g.getHeight(); h++) {
        for (int w = 0; w < g.getWidth(); w++) {
            float value = decode(s, byte, bitPos, root);
            g(w, h) = value / 255;
        }
    }
    g = g.decompTrans();
    return s;
}
std::istream & operator>>(std::istream& s, GreyScale& g) {
    char magicnumber[3];
    s.read(magicnumber, 3);
    if(!strncmp(magicnumber, "P2", 2)) {
        return GreyScale::readP2(s, g);
    } else if (!strncmp(magicnumber, "P5", 2)) {
        return GreyScale::readP5(s, g);
    } else if (!strncmp(magicnumber, "MHa", 3)) {
        return GreyScale::readMHa(s, g);
    } else if (!strncmp(magicnumber, "MHb", 3)) {
        return GreyScale::readMHb(s, g);
    } else {
        GreyScale::greyScaleError("ungueltige Magic Number");
    }

    return s;
}

std::ostream & GreyScale::writeP2(std::ostream& o, const GreyScale& g) {
    o << "P2\n";
    o << g.getWidth() << " " << g.getHeight()<< '\n';
    o << "255" << '\n';

    for(int h = 0; h < g.getHeight(); h++) {
        for(int w = 0; w < g.getWidth(); w++) {
            o <<  std::rint(g(w, h) * 255) << " ";
        }
        o << '\n';
    }

    return o;
}
std::ostream & GreyScale::writeP5(std::ostream& o, const GreyScale& g) {
    o << "P5\n";
    o << g.getWidth() << " " << g.getHeight()<< '\n';
    o << "255" << '\n';

    for(int h = 0; h < g.getHeight(); h++) {
        for(int w = 0; w < g.getWidth(); w++) {
            char b = std::rint(g(w, h) * 255);
            o << b;
        }
    }
    return o;
}
std::ostream & GreyScale::writeMHa(std::ostream& o, const GreyScale& g) {
    uint16_t width =  htons(g.getWidth());
    uint16_t height =  htons(g.getHeight());
    o.write((char*) &width, 2);
    o.write((char*) &height, 2);
    // create histogramm
    std::vector<Node> histogramm(256);

    for (int i = 0; i < 256; i++)
            histogramm[i].value = i;

    for(int h = 0; h < g.getHeight(); h++) {
        for(int w = 0; w < g.getWidth(); w++) {
            int greyValue = std::rint(g(w, h) * 255);
            histogramm[greyValue].freq += 1;
        }
    }
    //build tree
    Node root = buildTree(histogramm);
    //write hist
    for (int i = 0; i < 256; i++) {
        uint32_t freq = htonl(histogramm[i].freq);
        o.write((char*) &freq, 4);
    }
            
    // decode image
    std::vector< std::vector<int> > lt(256);
    std::vector<int> prefix;
    buildLookUpTable(root, prefix, lt);

    char byte = 0b00000000;
    int bitPos = 7;
    for(int h = 0; h < g.getHeight(); h++) {
        for(int w = 0; w < g.getWidth(); w++) {
            int greyValue = std::rint(g(w, h) * 255);
            for(auto bitVal : lt[greyValue]) {
                writeBit(o, byte, bitVal, bitPos);
            }
        }
    }
    writeBitFlush(o, byte, bitPos);
    return o;
}
std::ostream & GreyScale::writeMHb(std::ostream& o, const GreyScale& g) {
    GreyScale im = g.compTrans();
    return GreyScale::writeMHa(o, im);
}

std::ostream & operator<<(std::ostream& o,const GreyScale& g) {
    g.clamp();
    
    if(GreyScale::format == GreyScale::P2) {
        return GreyScale::writeP2(o, g);
    } else if(GreyScale::format == GreyScale::P5) {
        return GreyScale::writeP5(o, g);
    } else if(GreyScale::format == GreyScale::MHa) {
        o << "MHa";
        return GreyScale::writeMHa(o, g);
    } else if(GreyScale::format == GreyScale::MHb) {
        o << "MHb";
        return GreyScale::writeMHb(o, g);
    } else {
        GreyScale::greyScaleError("ungueltige Format");
        return o;
    }

}
