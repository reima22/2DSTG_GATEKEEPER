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
 26;
 -0.19574;-2.47191;-9.79344;,
 -0.19574;26.02647;1.17598;,
 12.10463;-2.47191;-9.73672;,
 -0.19574;26.02647;1.17598;,
 12.40847;-2.47191;1.17598;,
 -0.19574;26.02647;1.17598;,
 12.10463;-2.47191;12.08868;,
 -0.19574;26.02647;1.17598;,
 -0.19574;-2.47191;12.16790;,
 -0.19574;26.02647;1.17598;,
 -12.49610;-2.47191;12.08868;,
 -0.19574;26.02647;1.17598;,
 -12.93867;-2.47191;1.17598;,
 -0.19574;26.02647;1.17598;,
 -12.49610;-2.47191;-9.73672;,
 -0.19574;26.02647;1.17598;,
 -0.19574;-2.47191;-9.79344;,
 -0.19574;-2.47191;1.17598;,
 -0.19574;-2.47191;-9.79344;,
 12.10463;-2.47191;-9.73672;,
 12.40847;-2.47191;1.17598;,
 12.10463;-2.47191;12.08868;,
 -0.19574;-2.47191;12.16790;,
 -12.49610;-2.47191;12.08868;,
 -12.93867;-2.47191;1.17598;,
 -12.49610;-2.47191;-9.73672;;
 
 16;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;;
 
 MeshMaterialList {
  1;
  16;
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
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Ver.2.1\\data\\TEXTURE\\piramiddo.jpg";
   }
  }
 }
 MeshNormals {
  13;
  -0.000000;0.359222;-0.933252;,
  0.004303;0.359218;-0.933244;,
  0.914545;0.404483;0.000000;,
  0.914249;0.404352;0.025455;,
  0.000000;0.359863;0.933005;,
  -0.006009;0.359857;0.932988;,
  -0.912894;0.408197;-0.000000;,
  -0.912269;0.407917;-0.036997;,
  0.000000;-1.000000;-0.000000;,
  0.914249;0.404352;-0.025455;,
  0.006009;0.359857;0.932988;,
  -0.912269;0.407917;0.036997;,
  -0.004303;0.359218;-0.933244;;
  16;
  3;0,0,1;,
  3;9,2,2;,
  3;2,2,3;,
  3;10,4,4;,
  3;4,4,5;,
  3;11,6,6;,
  3;6,6,7;,
  3;12,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;;
 }
 MeshTextureCoords {
  26;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
