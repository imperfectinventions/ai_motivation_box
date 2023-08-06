import secrets
import requests
import json 
import random
import tiktoken

headers = {
    "Content-Type"  : "application/json",
    "Authorization" : f"Bearer {secrets.api_key}"
    }

celebs = [
    "Dwane \"The Rock\" Johnson",
    "Spongebob",
    "Nicki Minaj",
    "Stephen Hawking",
    "Lebron James",
    "Steve Jobs",
    "Master Shifu",
    "PewDiePie",
    "Michael Jackson",
    "The invisible Jon Cena"
]

#Unique and tied to person
#Ben Shapiro 
#Colleen Ballinger
#Andrew Tate
#"Old, Problematic PewDiePie before he got cancelled and is on a rage quit rant"
#"Shane Dawson talking to his pets" (this one gets sus furry stuff)

#for problematic inspiration:
#"Ben Shapiro talking about the woke left"
#"Infowars angry about the gay frogs taking our jobs" (global elitist stuff)
#"Jon Cena talking about how much he loves Bing Chilling" (goes on a rant about Bing Chilling)


celeb2use = celebs[random.randint(0, len(celebs)-1)]
print("In style of", celeb2use)
convo_content = f"Please generate 10 motivational quotes that meet the below requirements:" \
                f"- Put in the format: {{ \"msg1\" : \"place quote 1 here\", \"msg2\" : place quote 2 here\" ... and so on }}. This is in JSON format." \
                f"- Make sure each quote is at max 20 words long. Shoot for between 10 to 20 words." \
                f"- Make each quote in the style of {celeb2use}" \
                f"- Make all quotes generic and not specific to any particular party"


encoding = tiktoken.encoding_for_model("gpt-3.5-turbo")
for i in range(0, len(celebs)):
    convo_content = f"Please generate 10 motivational quotes that meet the below requirements:" \
                f"- Put in the JSON format: {{ \"msg1\" : \"place quote 1 here\", \"msg2\" : \"place quote 2 here\" ... and so on }}." \
                f"- Make each quote between 5 and 20 words. No more, no less." \
                f"- Make each quote in the style of {celebs[i]}, but also make the quote generic."
    print("Celeb:", celebs[i], "Num Tokens:", len(encoding.encode(convo_content)))

chat_data = {
    "model" : "gpt-3.5-turbo",
    "messages" : [
            {
                "role" : "user",
                "content" : convo_content,
            },
        ],
    "temperature" : 0.9,
    }

convo_result = requests.post('https://api.openai.com/v1/chat/completions', headers=headers, json=chat_data)
if convo_result.status_code != 200:
    print("Failed with status code:", convo_result.status_code)
    print(convo_result.reason)
    exit(1)

convo_json = convo_result.json()
#from:
#https://openai.com/pricing
cost_input = 0.0015 * (convo_json["usage"]["prompt_tokens"] / 1000)
cost_output = 0.002 * (convo_json["usage"]["completion_tokens"] / 1000)
print(f"Cost: ${cost_input + cost_output}")
print(f"Len of input chat_data: {len(str(chat_data))}")
print(f"Len of output convo result: {len(str(convo_json))}")

try:
    content_json = json.loads(convo_json["choices"][0]["message"]["content"])
    print(json.dumps(content_json, indent=4))
    print("Out Token Len:", len(encoding.encode(convo_json["choices"][0]["message"]["content"])))
except ValueError as e:
    print("Non JSON:")
    print(convo_json["choices"][0]["message"]["content"])
    print("Out Token Len:", len(encoding.encode(convo_json["choices"][0]["message"]["content"])))


