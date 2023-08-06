const char openai_json[] = \
"{\n" \
"    \"model\": \"gpt-3.5-turbo\",\n" \
"    \"messages\": [\n" \
"            {\n" \
"                \"role\": \"user\",\n" \
"                \"content\": \"%s\"\n" \
"            }\n" \
"        ],\n" \
"    \"temperature\": 0.9, \n" \
"    \"max_tokens\": 390\n" \
"}\n";

const char openai_prompt[] = \
"Please generate 5 motivational quotes that meet the below requirements: " \
"- Put in the JSON format: { \\\"msg1\\\" : \\\"place quote 1 here\\\", \\\"msg2\\\" : \\\"place quote 2 here\\\" ... and so on }. " \
"- Make each quote between 5 to 20 words. No more, no less. " \
"- Make each quote in the style of %s, but also make the quote generic. ";

const String openai_header_auth[] = { "Authorization", "Bearer %s" };
const String openai_header_content_type[] = { "Content-Type",  "application/json" };

const char openai_api_endpoint[] = "https://api.openai.com/v1/chat/completions";

//the max_tokens is 90 tokens (in prompt + longest celeb tokens) + max that I was seeing in result tokens + pad of 50
//total: 90 + 250 + 50 = 390
#define celeb_num 12
#define output_tokens 300
const String openai_celebs[12] = { \
    "Cardi B", \
    "Dwane \\\"The Rock\\\" Johnson", \
    "Spongebob", \
    "Nicki Minaj", \
    "Stephen Hawking", \
    "Lebron James", \
    "Steve Jobs", \
    "Master Shifu", \
    "Beyonce", \
    "PewDiePie", \
    "Michael Jackson", \
    "The invisible Jon Cena" \
};

//This is the cloudflare SSL cert on the OpenAI website. You can pull it from the OpenAI website itself
const char* openai_rootCACert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
"-----END CERTIFICATE-----\n";
