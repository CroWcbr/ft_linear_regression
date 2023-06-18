# ft_linear_regression

##
- predict : predict the price of a car for a given mileage.
- train   : train your model.
- bonus   : plotting the data and calculates the precision.

## Docker (for bonus - gnuplot)
- docker build -t docker_for_plot .
- docker run -it -v "$(pwd):/workspace" docker_for_plot
- connect using vs code
- exit
- docker rm $(docker ps -aq)
- docker rmi $(docker images -aq)
