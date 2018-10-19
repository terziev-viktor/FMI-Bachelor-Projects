function exponentfunc
    x=-1:0.01:3;
    y=x.^2;
    Y=0:0.01:3;
    X0=0:0:0;
    plot(x, y, 'g')
    hold on
    plot(X0, Y, 'b')
    grid on
end