#include <iostream>
#include <curl/curl.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <gumbo.h>
#include <string>
#include <vector>
#include <future>

// Callback to store HTTP response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to recursively extract text from Gumbo nodes
void extract_text(GumboNode* node, std::string& result) {
    if (node->type == GUMBO_NODE_TEXT) {
        result += node->v.text.text;
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        GumboVector* children = &node->v.element.children;
        for (size_t i = 0; i < children->length; ++i) {
            extract_text(static_cast<GumboNode*>(children->data[i]), result);
        }
    }
}

// Function to fetch and parse HTML from a given URL
std::string fetch_and_parse(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return "Error initializing CURL.";

    std::string htmlContent;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return "Error fetching content: " + std::string(curl_easy_strerror(res));
    }

    GumboOutput* output = gumbo_parse(htmlContent.c_str());
    std::string extractedText;
    extract_text(output->root, extractedText);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    curl_easy_cleanup(curl);
    return extractedText;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Simple Browser", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/SFNS.ttf", 16);

    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::string url = "https://example.com";
    std::string extractedText = fetch_and_parse(url);
    SDL_Rect urlRect = {10, 10, 780, 30};
    std::string typedUrl = url;

    bool running = true;
    SDL_Event event;
    SDL_StartTextInput();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_TEXTINPUT) typedUrl += event.text.text;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) extractedText = fetch_and_parse(typedUrl);
                else if (event.key.keysym.sym == SDLK_BACKSPACE && !typedUrl.empty()) typedUrl.pop_back();
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Surface* urlSurface = TTF_RenderText_Blended(font, typedUrl.c_str(), {0, 0, 0});
        SDL_Texture* urlTexture = SDL_CreateTextureFromSurface(renderer, urlSurface);
        SDL_RenderCopy(renderer, urlTexture, NULL, &urlRect);
        SDL_FreeSurface(urlSurface);
        SDL_DestroyTexture(urlTexture);

        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, extractedText.c_str(), {0, 0, 0}, 780);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {10, 50, 780, 500};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
