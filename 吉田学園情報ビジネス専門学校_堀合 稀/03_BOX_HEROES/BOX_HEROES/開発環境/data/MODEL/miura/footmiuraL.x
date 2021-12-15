xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 -5.90442;5.04326;1.98840;,
 -5.10266;5.78130;-1.53316;,
 -2.18110;5.33518;-1.12748;,
 -1.10142;5.08394;0.62940;,
 -1.26570;5.06130;5.92380;,
 -3.42322;5.25274;7.33480;,
 -7.64545;5.09190;2.90664;,
 -7.16326;5.67218;0.10880;,
 -6.55534;4.87594;4.94296;,
 -7.87193;7.37570;-7.30848;,
 -4.91758;7.37594;6.92156;,
 -0.06886;7.33610;5.89184;,
 -1.85010;7.42897;-7.97652;,
 -7.66749;0.07458;-8.39508;,
 -1.24326;0.46998;-8.18364;,
 -0.19086;-0.45358;5.05584;,
 -4.38570;-0.45094;5.97228;,
 -7.87193;7.37570;-7.30848;,
 -1.85010;7.42897;-7.97652;,
 -1.24326;0.46998;-8.18364;,
 -7.66749;0.07458;-8.39508;,
 -4.91758;7.37594;6.92156;,
 -0.19086;-0.45358;5.05584;,
 -0.06886;7.33610;5.89184;,
 -1.85010;7.42897;-7.97652;,
 -0.06886;7.33610;5.89184;,
 -7.87193;7.37570;-7.30848;,
 -7.66749;0.07458;-8.39508;;
 
 10;
 4;0,1,2,3;,
 4;4,5,0,3;,
 4;6,7,1,0;,
 4;5,8,6,0;,
 4;9,10,11,12;,
 4;13,14,15,16;,
 4;17,18,19,20;,
 4;21,16,22,23;,
 4;15,19,24,25;,
 4;21,26,27,16;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  15;
  -0.139873;-0.963401;-0.228679;,
  -0.071588;-0.981514;-0.177499;,
  0.036950;-0.998145;-0.048386;,
  -0.020490;-0.999377;0.028733;,
  0.008240;-0.999918;-0.009838;,
  -0.105831;-0.973384;-0.203283;,
  -0.046302;-0.996122;-0.074810;,
  -0.049116;-0.993017;-0.107258;,
  -0.051885;-0.988838;-0.139671;,
  0.000367;0.999993;0.003635;,
  0.023749;-0.997937;-0.059649;,
  -0.039908;0.088213;-0.995302;,
  0.208963;-0.105798;0.972184;,
  0.994012;0.043526;-0.100228;,
  -0.974495;-0.075436;0.211351;;
  10;
  4;7,5,1,6;,
  4;3,4,7,6;,
  4;8,0,5,7;,
  4;4,2,8,7;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.500000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.500000;0.767470;,
  0.000000;0.500000;,
  0.000000;0.750000;,
  0.750000;0.500000;;
 }
}
