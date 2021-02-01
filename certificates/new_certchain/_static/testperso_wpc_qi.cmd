@echo off
set OPENSSL=%1
set CSR=%2
)

REM Create Manfucating CA
call :genkey IFX_WPC_QI_13_Manufacturing_key
call :sign IFX_WPC_QI_13_Root_key.pem IFX_WPC_QI_13_CA_Config.cfg IFX_WPC_QI_13_Root_Certificate.pem 1234ABCD IFX_WPC_QI_13_Manufacturing_key.pem IFX_WPC_QI_13_Manufacturing_Request.conf IFX_WPC_QI_13_Manufacturing_Certificate

REM Create Product Unit Certificate from Trust Sculptor generated CSR
call :sign_sculptor IFX_WPC_QI_13_Manufacturing_key.pem IFX_WPC_QI_13_CA_Config.cfg IFX_WPC_QI_13_Manufacturing_Certificate.pem 1234EF01 %CSR% IFX_WPC_QI_13_ProductUnit_Certificate

exit /b


@rem SUBROUTINE
:genkey
set NAME=%1
REM   Generate Key (uncompressed)
%OPENSSL% ecparam -name prime256v1 -param_enc named_curve -genkey -noout -out %NAME%.pem
REM   Compress Key
%OPENSSL% ec -in %NAME%.pem -conv_form compressed -out %NAME%_comp.pem
del %NAME%.pem
move %NAME%_comp.pem %NAME%.pem
REM   Parse key 
%OPENSSL% asn1parse -in %NAME%.pem  > %NAME%_asn1.txt
%OPENSSL% ec -in %NAME%.pem -outform DER -out %NAME%.der

exit /b

@rem SUBROUTINE
:sign_sculptor
set CAKEY=%1
set CACFG=%2
set CACERT=%3
set SERIAL=%4
set CSR=%5
set CERTNAME=%6

REM pretty print for debugging
%OPENSSL% req -inform DER -in %CSR% -text > %CSR%.txt
REM OpenSSL seems to need CSRs in PEM format
%OPENSSL% req -inform DER -in %CSR% -out %CSR%.pem


@REM Sign CSR / Create cert
@del serial.txt*
@del certindex.txt*
@echo %SERIAL% > serial.txt
@copy NUL certindex.txt
@copy NUL certindex.txt.attr

REM 
%OPENSSL% ca -keyfile %CAKEY% -batch -cert %CACERT% -config %CACFG% -infiles %CSR%.pem
@move %SERIAL%.pem %CERTNAME%.pem
@%OPENSSL% x509 -in %CERTNAME%.pem -outform DER -out %CERTNAME%.crt

@REM Crt to txt
@%OPENSSL% x509 -inform DER -in %CERTNAME%.crt -text > %CERTNAME%.txt

exit /b

@rem SUBROUTINE
:sign

set CAKEY=%1
set CACFG=%2
set CACERT=%3
set SERIAL=%4
set CERTKEY=%5
set REQCFG=%6
set CERTNAME=%7

%OPENSSL% req -new -sha256 -key %CERTKEY% -config %REQCFG% -out %CERTNAME%.csr
%OPENSSL% req -in %CERTNAME%.csr -text > %CERTNAME%.csr.txt

@REM Sign CSR / Create cert
@del serial.txt*
@del certindex.txt*
@echo %SERIAL% > serial.txt
@copy NUL certindex.txt
@copy NUL certindex.txt.attr

REM 
%OPENSSL% ca -keyfile %CAKEY% -batch -extensions %CERTNAME% -cert %CACERT% -config %CACFG%  -infiles %CERTNAME%.csr
@move %SERIAL%.pem %CERTNAME%.pem
@%OPENSSL% x509 -in %CERTNAME%.pem -outform DER -out %CERTNAME%.crt

@REM Crt to txt
@%OPENSSL% x509 -inform DER -in %CERTNAME%.crt -text > %CERTNAME%.txt

exit /b


:selfsign

set CAKEY=%1
set CACFG=%2
set SERIAL=%3
set CERTKEY=%4
set REQCFG=%5
set CERTNAME=%6

%OPENSSL% req -new -sha256 -key %CERTKEY% -config %REQCFG% -out %CERTNAME%.csr
%OPENSSL% req -in %CERTNAME%.csr -text > %CERTNAME%.csr.txt

@REM Sign CSR / Create cert
@del serial.txt*
@del certindex.txt*
@echo %SERIAL% > serial.txt
@copy NUL certindex.txt
@copy NUL certindex.txt.attr

echo CA
%OPENSSL% ca -keyfile %CAKEY% -batch -extensions %CERTNAME% -selfsign -config %CACFG%  -infiles %CERTNAME%.csr
@move %SERIAL%.pem %CERTNAME%.pem
@%OPENSSL% x509 -in %CERTNAME%.pem -outform DER -out %CERTNAME%.crt

@REM Crt to txt
@%OPENSSL% x509 -inform DER -in %CERTNAME%.crt -text > %CERTNAME%.txt

exit /b