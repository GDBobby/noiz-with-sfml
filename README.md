# noiz-with-sfml

Using the noiz library (authored by kaulkarn, with a little assitance from me)
https://github.com/cpp-gamedev/noiz

I'm using an experimental branch that I'm updating as I go, so I'm just directly including the file. When this project reaches a later stage, I'll directly link to the lib through github

## Functionality

Water can be set manually
  Beach is water + 5
  Land is Water + 10

All processed noises are casted to 0 ~ 100
For example, multi-fractal noise typically ranges from 0 to 4, I'm multiplyign that value by 25 to get a uint8_t value from 0 to 100.

Currently only supports 2d generation. 
The plan is to rebase with vulkan and imgui
two fold, we'll get a decent GUI instead of my awful SFML gui implementation,
and second we'll get 3d support.
