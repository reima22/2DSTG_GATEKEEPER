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
 23;
 2.40840;34.83578;3.52519;,
 14.28263;23.63764;-0.33298;,
 -1.19104;23.63764;-13.09282;,
 2.40840;34.83578;3.52519;,
 9.74708;23.63764;13.62602;,
 2.40840;34.83578;3.52519;,
 -17.72829;23.63764;4.42745;,
 2.40840;34.83578;3.52519;,
 -9.46584;23.63764;-0.33298;,
 2.40840;34.83578;3.52519;,
 -1.19104;23.63764;-13.09282;,
 21.62130;13.01700;-2.71747;,
 -1.19104;13.01700;-20.80916;,
 14.28263;13.01700;19.86867;,
 -22.26384;13.01700;10.67011;,
 -16.80451;13.01700;-2.71747;,
 -1.19104;13.01700;-20.80916;,
 30.02333;-0.11084;-3.57795;,
 -1.52409;-0.11084;-28.59710;,
 19.87460;-0.11084;27.65662;,
 -30.66587;-0.11084;14.93585;,
 -23.11612;-0.11084;-3.57795;,
 -1.52409;-0.11084;-28.59710;;
 
 15;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 4;2,1,11,12;,
 4;1,4,13,11;,
 4;4,6,14,13;,
 4;6,8,15,14;,
 4;8,10,16,15;,
 4;12,11,17,18;,
 4;11,13,19,17;,
 4;13,14,20,19;,
 4;14,15,21,20;,
 4;15,16,22,21;;
 
 MeshMaterialList {
  1;
  15;
  0,
  0,
  0,
  0,
  0,
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
   0.520800;0.520800;0.520800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  23;
  0.642481;0.744210;-0.182674;,
  0.505476;0.600556;-0.619538;,
  0.713994;0.668687;-0.207533;,
  0.766988;0.591289;0.249210;,
  -0.254439;0.520938;0.814791;,
  -0.606609;0.569364;-0.554843;,
  0.554953;0.459889;-0.693203;,
  0.792922;0.557582;-0.245719;,
  0.819793;0.506940;0.266367;,
  -0.231668;0.462593;0.855768;,
  -0.743873;0.416431;-0.522722;,
  0.566849;0.409641;-0.714756;,
  0.819676;0.507148;0.266329;,
  -0.220787;0.426344;0.877202;,
  -0.778501;0.405679;-0.478916;,
  -0.266513;0.543388;0.796053;,
  -0.485783;0.696890;-0.527597;,
  -0.566447;0.503583;-0.652336;,
  -0.637018;0.625580;-0.450397;,
  -0.787521;0.388839;-0.478136;,
  -0.697168;0.441880;-0.564535;,
  -0.838043;0.425315;-0.341746;,
  -0.701250;0.376815;-0.605193;;
  15;
  3;0,2,1;,
  3;0,3,2;,
  3;15,4,4;,
  3;16,5,17;,
  3;16,18,5;,
  4;1,2,7,6;,
  4;2,3,8,7;,
  4;4,4,9,9;,
  4;17,5,10,19;,
  4;5,18,20,10;,
  4;6,7,11,11;,
  4;7,8,12,12;,
  4;9,9,13,13;,
  4;19,10,14,21;,
  4;10,20,22,14;;
 }
 MeshTextureCoords {
  23;
  0.100000;0.000000;,
  0.200000;0.200000;,
  0.000000;0.200000;,
  0.300000;0.000000;,
  0.400000;0.200000;,
  0.500000;0.000000;,
  0.600000;0.200000;,
  0.700000;0.000000;,
  0.800000;0.200000;,
  0.900000;0.000000;,
  1.000000;0.200000;,
  0.200000;0.400000;,
  0.000000;0.400000;,
  0.400000;0.400000;,
  0.600000;0.400000;,
  0.800000;0.400000;,
  1.000000;0.400000;,
  0.200000;0.600000;,
  0.000000;0.600000;,
  0.400000;0.600000;,
  0.600000;0.600000;,
  0.800000;0.600000;,
  1.000000;0.600000;;
 }
}
