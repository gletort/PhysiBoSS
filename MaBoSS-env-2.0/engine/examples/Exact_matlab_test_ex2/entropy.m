function H=entropy(V)
    if (sum(V.*V) > 0)
        V=V/sum(V);     
        H=0;
        for i=1:length(V)
            if V(i)>0
                H=H-log2(V(i))*V(i);
            end
        end
    else
        H=0;
    end
end