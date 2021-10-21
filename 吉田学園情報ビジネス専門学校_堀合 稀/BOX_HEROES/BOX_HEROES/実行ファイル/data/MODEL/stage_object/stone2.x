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
 10.22533;131.00607;14.96688;,
 56.10374;87.73991;0.06008;,
 -3.68180;87.73991;-49.24001;,
 10.22533;131.00607;14.96688;,
 38.57976;87.73991;126.38143;,
 10.22533;131.00607;14.96688;,
 -67.57669;87.73991;18.45295;,
 10.22533;131.00607;14.96688;,
 -35.65310;87.73991;0.06008;,
 10.22533;131.00607;14.96688;,
 -3.68180;87.73991;-49.24001;,
 102.14962;51.99724;-18.51754;,
 -4.38307;51.99724;-103.00499;,
 67.87833;51.99724;174.45260;,
 -102.79215;51.99724;44.00188;,
 -77.29732;51.99724;-18.51754;,
 -4.38307;51.99724;-103.00499;,
 141.38682;-9.30927;-22.53599;,
 -5.93837;-9.30927;-139.37429;,
 93.99261;-9.30927;281.26661;,
 -142.02935;-9.30927;152.73157;,
 -106.77227;-9.30927;-22.53599;,
 -5.93837;-9.30927;-139.37429;;
 
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
  0.620711;0.743709;-0.248222;,
  0.126649;0.825799;-0.549560;,
  0.591724;0.772994;-0.228787;,
  0.678775;0.727396;0.100795;,
  -0.573129;0.795396;0.197152;,
  -0.523549;0.700647;-0.484758;,
  -0.081277;0.716948;-0.692373;,
  0.678960;0.683157;-0.268904;,
  0.748129;0.652199;0.122229;,
  -0.401203;0.750336;0.525387;,
  -0.701743;0.549758;-0.453125;,
  0.566848;0.409641;-0.714757;,
  0.780001;0.535886;-0.323148;,
  0.825665;0.547061;0.137850;,
  -0.292513;0.838930;0.458948;,
  -0.815404;0.370083;-0.445146;,
  -0.544967;0.838021;0.027040;,
  -0.466723;0.727472;0.502944;,
  -0.384539;0.637709;-0.667425;,
  -0.570849;0.732861;-0.370198;,
  -0.764605;0.517085;-0.384710;,
  -0.898008;0.349034;-0.267875;,
  -0.701251;0.376814;-0.605193;;
  15;
  3;0,2,1;,
  3;0,3,2;,
  3;16,4,17;,
  3;18,5,18;,
  3;16,19,5;,
  4;1,2,7,6;,
  4;2,3,8,7;,
  4;17,4,9,9;,
  4;4,5,10,20;,
  4;5,1,6,10;,
  4;6,7,12,11;,
  4;7,8,13,12;,
  4;9,9,14,14;,
  4;20,10,15,21;,
  4;10,6,22,15;;
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
