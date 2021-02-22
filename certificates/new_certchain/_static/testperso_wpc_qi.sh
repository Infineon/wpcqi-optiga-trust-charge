#!/bin/bash

OPENSSL=$1
CSR=$2
STORAGE=..

generate_manufacturer_ca_key () {
  NAME=$1
  # Generate Key (uncompressed)
  $OPENSSL ecparam -name prime256v1 -param_enc named_curve -genkey -noout -out $NAME.pem
  # Compress Key
  $OPENSSL ec -in $NAME.pem -conv_form compressed -out $NAME_comp.pem
  rm $NAME.pem
  mv $NAME_comp.pem $NAME.pem
  # Parse key
  $OPENSSL asn1parse -in $NAME.pem  > $NAME_asn1.txt
  $OPENSSL ec -in $NAME.pem -outform DER -out $NAME.der
}

sign_manufacturer_ca_cert () {
  CAKEY=$1
  CACFG=$2
  CACERT=$3
  SERIAL=$4
  CERTKEY=$5
  REQCFG=$6
  CERTNAME=$7

  $OPENSSL req -new -sha256 -key $CERTKEY -config $REQCFG -out $STORAGE/$CERTNAME.csr
  $OPENSSL req -in $STORAGE/$CERTNAME.csr -text > $STORAGE/$CERTNAME.csr.txt

  # Sign CSR / Create cert
  rm serial.txt*
  rm certindex.txt*
  echo $SERIAL > serial.txt
  cp /dev/null certindex.txt
  cp /dev/null certindex.txt.attr

  $OPENSSL ca -keyfile $CAKEY -batch -extensions $CERTNAME -cert $CACERT -config $CACFG  -infiles $STORAGE/$CERTNAME.csr
  mv $SERIAL.pem $STORAGE/$CERTNAME.pem
  $OPENSSL x509 -in $STORAGE/$CERTNAME.pem -outform DER -out $STORAGE/$CERTNAME.crt

  #REM Crt to txt
  $OPENSSL x509 -inform DER -in $STORAGE/$CERTNAME.crt -text > $STORAGE/$CERTNAME.txt
}

sign_product_unit_csr () {
  # Create Product Unit Certificate from Trust Sculptor generated CSR
  CAKEY=$1
  CACFG=$2
  CACERT=$3
  SERIAL=$4
  CSR=$5
  CERTNAME=$6

  # pretty print for debugging
  $OPENSSL req -inform DER -in $CSR -text > $CSR.txt
  # REM OpenSSL seems to need CSRs in PEM format
  $OPENSSL req -inform DER -in $CSR -out $CSR.pem

  # REM Sign CSR / Create cert
  rm serial.txt*
  rm certindex.txt*
  echo $SERIAL > serial.txt
  cp /dev/null certindex.txt
  cp /dev/null certindex.txt.attr

  $OPENSSL ca -keyfile $CAKEY -batch -cert $CACERT -config $CACFG -infiles $CSR.pem
  mv $SERIAL.pem $CERTNAME.pem
  $OPENSSL x509 -in $CERTNAME.pem -outform DER -out $CERTNAME.crt

  # Crt to txt
  $OPENSSL x509 -inform DER -in $CERTNAME.crt -text > $CERTNAME.txt
}

generate_manufacturer_ca_key $STORAGE/IFX_WPC_QI_13_Manufacturing_key

sign_manufacturer_ca_cert \
  IFX_WPC_QI_13_Root_key.pem \
  IFX_WPC_QI_13_CA_Config.cfg \
  IFX_WPC_QI_13_Root_Certificate.pem \
  1234ABCD \
  $STORAGE/IFX_WPC_QI_13_Manufacturing_key.pem \
  IFX_WPC_QI_13_Manufacturing_Request.conf \
  IFX_WPC_QI_13_Manufacturing_Certificate

sign_product_unit_csr \
  $STORAGE/IFX_WPC_QI_13_Manufacturing_key.pem \
  IFX_WPC_QI_13_CA_Config.cfg \
  $STORAGE/IFX_WPC_QI_13_Manufacturing_Certificate.pem \
  1234EF01 \
  $CSR \
  $STORAGE/IFX_WPC_QI_13_ProductUnit_Certificate