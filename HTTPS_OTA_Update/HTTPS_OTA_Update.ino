// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"
// This sketch shows how to implement HTTPS firmware update Over The Air.
// Please provide your WiFi credentials, https URL to the firmware image and the server certificate.

static const char *ssid = "";          // your network SSID (name of wifi network)
static const char *password = "";  // your network password

static const char *url = "https://raw.githubusercontent.com/codyn95/ota-test/main/ota/ota.ino.bin";  //state url of your firmware image

static const char *server_certificate = R"string_literal(-----BEGIN CERTIFICATE-----
MIIEyDCCA7CgAwIBAgIQDPW9BitWAvR6uFAsI8zwZjANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH
MjAeFw0yMTAzMzAwMDAwMDBaFw0zMTAzMjkyMzU5NTlaMFkxCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxMzAxBgNVBAMTKkRpZ2lDZXJ0IEdsb2Jh
bCBHMiBUTFMgUlNBIFNIQTI1NiAyMDIwIENBMTCCASIwDQYJKoZIhvcNAQEBBQAD
ggEPADCCAQoCggEBAMz3EGJPprtjb+2QUlbFbSd7ehJWivH0+dbn4Y+9lavyYEEV
cNsSAPonCrVXOFt9slGTcZUOakGUWzUb+nv6u8W+JDD+Vu/E832X4xT1FE3LpxDy
FuqrIvAxIhFhaZAmunjZlx/jfWardUSVc8is/+9dCopZQ+GssjoP80j812s3wWPc
3kbW20X+fSP9kOhRBx5Ro1/tSUZUfyyIxfQTnJcVPAPooTncaQwywa8WV0yUR0J8
osicfebUTVSvQpmowQTCd5zWSOTOEeAqgJnwQ3DPP3Zr0UxJqyRewg2C/Uaoq2yT
zGJSQnWS+Jr6Xl6ysGHlHx+5fwmY6D36g39HaaECAwEAAaOCAYIwggF+MBIGA1Ud
EwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFHSFgMBmx9833s+9KTeqAx2+7c0XMB8G
A1UdIwQYMBaAFE4iVCAYlebjbuYP+vq5Eu0GF485MA4GA1UdDwEB/wQEAwIBhjAd
BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwdgYIKwYBBQUHAQEEajBoMCQG
CCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wQAYIKwYBBQUHMAKG
NGh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RH
Mi5jcnQwQgYDVR0fBDswOTA3oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQuY29t
L0RpZ2lDZXJ0R2xvYmFsUm9vdEcyLmNybDA9BgNVHSAENjA0MAsGCWCGSAGG/WwC
ATAHBgVngQwBATAIBgZngQwBAgEwCAYGZ4EMAQICMAgGBmeBDAECAzANBgkqhkiG
9w0BAQsFAAOCAQEAkPFwyyiXaZd8dP3A+iZ7U6utzWX9upwGnIrXWkOH7U1MVl+t
wcW1BSAuWdH/SvWgKtiwla3JLko716f2b4gp/DA/JIS7w7d7kwcsr4drdjPtAFVS
slme5LnQ89/nD/7d+MS5EHKBCQRfz5eeLjJ1js+aWNJXMX43AYGyZm0pGrFmCW3R
bpD0ufovARTFXFZkAdl9h6g4U5+LXUZtXMYnhIHUfoyMo5tS58aI7Dd8KvvwVVo4
chDYABPPTHPbqjc1qCmBaZx2vN4Ye5DUys/vZwP9BFohFrH/6j/f3IL16/RZkiMN
JCqVJUzKoZHm1Lesh3Sz8W2jmdv51b2EQJ8HmA==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIHOTCCBiGgAwIBAgIQBj1JF0BNOeUTyz/uzRsuGzANBgkqhkiG9w0BAQsFADBZ
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTMwMQYDVQQDEypE
aWdpQ2VydCBHbG9iYWwgRzIgVExTIFJTQSBTSEEyNTYgMjAyMCBDQTEwHhcNMjQw
MzE1MDAwMDAwWhcNMjUwMzE0MjM1OTU5WjBnMQswCQYDVQQGEwJVUzETMBEGA1UE
CBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEVMBMGA1UEChMM
R2l0SHViLCBJbmMuMRQwEgYDVQQDDAsqLmdpdGh1Yi5pbzCCASIwDQYJKoZIhvcN
AQEBBQADggEPADCCAQoCggEBAK0rFKU6TEGvuLCY3ZOuXlG+3jerD6EP1gc1qe35
g68FqyGuVPOUddYNZiymjYMZxywoNp3qxlbFFBTf9etsayavT+uW+2UMjqCotAdK
KicBEspuExoACFuNgTi7sSUT7A55+k4/+5O+VtpaxQ5dmQk7HxcqvMYx5owBU+fB
wYDD+hXeg3YvxLZNeIlN8OlqWL8w9HbG+3ccegVEjOJQbkrcrW7IQMq2Uk92XjxI
PmMVIvaefqcC1poGYvS4VvEh3x64vJK1hEM4YLMKBaE/hqFtcMozi+H/8JqTCfzP
Qhnu21HIop9rSucxxnZbe9AeHz2LERpUTf3rjgOMg9PB1RUCAwEAAaOCA+0wggPp
MB8GA1UdIwQYMBaAFHSFgMBmx9833s+9KTeqAx2+7c0XMB0GA1UdDgQWBBTob1fr
hlGY65+lvlPa25SsKC777TB7BgNVHREEdDByggsqLmdpdGh1Yi5pb4IJZ2l0aHVi
LmlvghVnaXRodWJ1c2VyY29udGVudC5jb22CDnd3dy5naXRodWIuY29tggwqLmdp
dGh1Yi5jb22CFyouZ2l0aHVidXNlcmNvbnRlbnQuY29tggpnaXRodWIuY29tMD4G
A1UdIAQ3MDUwMwYGZ4EMAQICMCkwJwYIKwYBBQUHAgEWG2h0dHA6Ly93d3cuZGln
aWNlcnQuY29tL0NQUzAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUH
AwEGCCsGAQUFBwMCMIGfBgNVHR8EgZcwgZQwSKBGoESGQmh0dHA6Ly9jcmwzLmRp
Z2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbEcyVExTUlNBU0hBMjU2MjAyMENBMS0x
LmNybDBIoEagRIZCaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xv
YmFsRzJUTFNSU0FTSEEyNTYyMDIwQ0ExLTEuY3JsMIGHBggrBgEFBQcBAQR7MHkw
JAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBRBggrBgEFBQcw
AoZFaHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsRzJU
TFNSU0FTSEEyNTYyMDIwQ0ExLTEuY3J0MAwGA1UdEwEB/wQCMAAwggF/BgorBgEE
AdZ5AgQCBIIBbwSCAWsBaQB2AE51oydcmhDDOFts1N8/Uusd8OCOG41pwLH6ZLFi
mjnfAAABjkN89oAAAAQDAEcwRQIgU/M527Wcx0KQ3II7kCuG5WMuOHRSxKkf1xAj
JuSkyPACIQCVX0uurcIA2Ug7ipNN2S1ZygukWqJCh7hjIH0XsrXh8QB2AH1ZHhLh
eCp7HGFnfF79+NCHXBSgTpWeuQMv2Q6MLnm4AAABjkN89oEAAAQDAEcwRQIgCxpL
BDak+TWKarrCHlZn4DlqwEfAN3lvlgSo21HQuU8CIQDicrb72c0lA2suMWPWT92P
FLaRvFrFn9HVzI6Vh50YZgB3AObSMWNAd4zBEEEG13G5zsHSQPaWhIb7uocyHf0e
N45QAAABjkN89pQAAAQDAEgwRgIhAPJQX4QArFCjM0sKKzsWLmqmmU8lMhKEYR2T
ges1AQyQAiEA2Y3VhP5RG+dapcbwYgVbrTlgWzO7KE/lg1x11CVcz3QwDQYJKoZI
hvcNAQELBQADggEBAHKlvzObJBxxgyLaUNCEFf37mNFsUtXmaWvkmcfIt9V+TZ7Q
mtvjx5bsd5lqAflp/eqk4+JYpnYcKWrZfM/vMdxPQTeh/VQWewY/hYn6X/V1s2JI
MtjqEkW4aotVdWjHVvsx4rAjz5vtub/wVYgtrU8jusH3TVpT9/0AoFhKE5m2IS7M
Ig7wKR+DDxoNj4fFFluxteVNgbtwuJcb23NkBQqfHXCvQWqxXZZA4Nwl/WoGPoGG
dW5qVOc3BlhtITW53ASyhvKC7HArhj7LwQH8C/dRgn1agIHP9vVJ1NaZnPXhK98T
ohv++OO0E/F/bVGNWVnLBQ4v5PjQzRQUTGvM2mU=
-----END CERTIFICATE-----
)string_literal";

static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event) {
  switch (event->event_id) {
    case HTTP_EVENT_ERROR:        Serial.println("Http Event Error"); break;
    case HTTP_EVENT_ON_CONNECTED: Serial.println("Http Event On Connected"); break;
    case HTTP_EVENT_HEADER_SENT:  Serial.println("Http Event Header Sent"); break;
    case HTTP_EVENT_ON_HEADER:    Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value); break;
    case HTTP_EVENT_ON_DATA:      break;
    case HTTP_EVENT_ON_FINISH:    Serial.println("Http Event On Finish"); break;
    case HTTP_EVENT_DISCONNECTED: Serial.println("Http Event Disconnected"); break;
    case HTTP_EVENT_REDIRECT:     Serial.println("Http Event Redirect"); break;
  }
}

void setup() {

  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  HttpsOTA.onHttpEvent(HttpEvent);
  Serial.println("Starting OTA");
  HttpsOTA.begin(url, server_certificate);

  Serial.println("Please Wait it takes some time ...");
}

void loop() {
  otastatus = HttpsOTA.status();
  if (otastatus == HTTPS_OTA_SUCCESS) {
    Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
  } else if (otastatus == HTTPS_OTA_FAIL) {
    Serial.println("Firmware Upgrade Fail");
  }
  delay(1000);
}
